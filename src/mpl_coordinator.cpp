#include <jilog.hpp>
#include <mpl/buffer.hpp>
#include <mpl/write_queue.hpp>
#include <mpl/packet.hpp>
#include <mpl/syserr.hpp>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <vector>
#include <list>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

namespace mpl {
    // template <class S>
    // std::pair<int, int> launchLambda(std::uint64_t pId, packet::ProblemSE3<S>& prob);

    std::pair<int, int> launchLambda(std::uint64_t pId, packet::Problem& prob);

    class Coordinator {
        using ID = std::uint64_t;
        
        int listen_{-1};

        class GroupData;
        class Connection;

        using Group = std::pair<const ID, GroupData>;
        
        ID nextGroupId_{1};

        std::list<Connection> connections_;
        std::list<std::pair<int, int>> childProcesses_;
        std::map<ID, GroupData> groups_;
        
    public:
        Coordinator(int port = 0x415E)
            : listen_(::socket(PF_INET, SOCK_STREAM, 0))
        {
            if (listen_ == -1)
                throw syserr("socket()");

            int on = 1;
            if (::setsockopt(listen_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
                throw syserr("set reuse addr");
            
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            addr.sin_port = htons(port);
            
            if (::bind(listen_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1)
                throw syserr("bind()");

            socklen_t addrLen = sizeof(addr);
            if (::getsockname(listen_, reinterpret_cast<struct sockaddr*>(&addr), &addrLen) == -1)
                throw syserr("getsockname()");

            JI_LOG(INFO) << "listening on port: " << ntohs(addr.sin_port);
            
            if (::listen(listen_, 5) == -1)
                throw syserr("listen()");
        }
        
        ~Coordinator() {
            if (listen_ != -1 && !::close(listen_))
                JI_LOG(WARN) << "failed to close listening socket";                
        }

        int accept(struct sockaddr *addr, socklen_t * addrLen) {
            return ::accept(listen_, addr, addrLen);
        }

        void loop();

        void launchLambdas(ID groupId, packet::Problem&& prob, int nLambdas);
        
        Group* createGroup(Connection* initiator);
        Group* addToGroup(ID id, Connection* conn);
        void done(Group* group, Connection* conn);
        
        template <class T>
        void broadcast(T&& packet, Group* group, Connection* conn);
    };

    class Coordinator::GroupData {
        Connection* initiator_;
        bool done_{false};
        std::list<Connection*> connections_;
        
    public:
        GroupData(Connection* initiator)
            : initiator_(initiator)
        {
        }

        bool isDone() const {
            return done_;
        }

        void done() {
            done_ = true;
        }

        Connection* initiator() {
            return initiator_;
        }

        auto& connections() {
            return connections_;
        }
    };

    class Coordinator::Connection {
        Coordinator& coordinator_;
        
        struct sockaddr_in addr_;
        socklen_t addrLen_{sizeof(addr_)};
        
        int socket_{-1};

        Buffer rBuf_{1024*4};
        WriteQueue writeQueue_;

        Group* group_{nullptr};

        bool doRead() {
            assert(rBuf_.remaining() > 0); // we may need to grow the buffer
            
            ssize_t n = ::recv(socket_, rBuf_.begin(), rBuf_.remaining(), 0);
            JI_LOG(TRACE) << "recv " << n;
            if (n <= 0) {
                // on error (-1) or connection close (0), send DONE to
                // the group to which this connection is attached.
                if (group_)
                    coordinator_.done(group_, this);
                
                return (n < 0) ? throw syserr("recv") : false;
            }

            rBuf_ += n;
            rBuf_.flip();
            // call the appropriate process overload for each packet
            // that arrives
            std::size_t needed;
            while ((needed = packet::parse(rBuf_, [&] (auto&& pkt) {
                            process(std::forward<decltype(pkt)>(pkt));
                        })) == 0);
            rBuf_.compact(needed);
            return true;
        }

        void process(packet::Hello&& pkt) {
            JI_LOG(INFO) << "got HELLO (id=" << pkt.id() << ")";
            group_ = coordinator_.addToGroup(pkt.id(), this);
            // this is a possible sign that the group already ended
            // before this connection arrived.  Respond with DONE.
            if (group_ == nullptr)
                writeQueue_.push_back(packet::Done(pkt.id()));
        }

        void process(packet::Done&& pkt) {
            JI_LOG(INFO) << "got DONE (id=" << pkt.id() << ")";
            if (group_ == nullptr || group_->first != pkt.id()) {
                JI_LOG(WARN) << "DONE group id mismatch";
            } else {
                coordinator_.done(group_, this);
                group_ = nullptr;
            }
        }

        void process(packet::Problem&& pkt) {
            JI_LOG(INFO) << "got Problem from " << socket_;

            // if this connection is connected to a group, send DONE
            // to that group before starting a new group.
            if (group_)
                coordinator_.done(group_, this);
            
            group_ = coordinator_.createGroup(this);
            int nLambdas = 4;
            coordinator_.launchLambdas(group_->first, std::move(pkt), nLambdas);
        }

        template <class State>
        void process(packet::Path<State>&& pkt) {
            JI_LOG(INFO) << "got Path " << sizeof(State);
            for (auto& q : pkt.path())
                JI_LOG(TRACE) << "  " << q;

            if (group_ == nullptr) {
                JI_LOG(WARN) << "got PATH without active group";
            } else {
                coordinator_.broadcast(std::move(pkt), group_, this);
            }
        }
        
    public:
        explicit Connection(Coordinator& coordinator)
            : coordinator_(coordinator)
            , socket_(coordinator.accept(reinterpret_cast<struct sockaddr*>(&addr_), &addrLen_))
        {
            JI_LOG(TRACE) << "connection accepted";
        }
        
        ~Connection() {
            if (group_)
                coordinator_.done(group_, this);
            
            JI_LOG(TRACE) << "closing connection";
            if (socket_ != -1 && ::close(socket_) == -1)
                JI_LOG(WARN) << "connection close error: " << errno;
        }
        
        operator bool () const {
            return socket_ != -1;
        }

        operator struct pollfd () const {
            return { socket_, static_cast<short>(writeQueue_.empty() ? POLLIN : (POLLIN | POLLOUT)), 0 };
        }

        void degroup() {
            group_ = nullptr;
        }

        template <class Packet>
        void write(Packet&& packet) {
            writeQueue_.push_back(std::forward<Packet>(packet));
        }

        bool process(const struct pollfd& pfd) {
            try {
                if ((pfd.revents & POLLIN) && !doRead())
                    return false;

                if (pfd.revents & POLLOUT)
                    writeQueue_.writeTo(socket_);

                return true;
            } catch (const std::exception& ex) {
                JI_LOG(WARN) << "exception processing connection: " << ex.what();
                return false;
            }
        }
    };

    template <class ... T>
    std::string to_string(T&& ... args) {
        std::ostringstream str;
        (str << ... << std::forward<T>(args));
        return str.str();
    }
}

// returns a pair of process-id and pipe-fd associated with the
// child process
std::pair<int, int> mpl::launchLambda(std::uint64_t pId, packet::Problem& prob) {
    // static const std::string resourceDirectory = "../../resources/";
    static int lambdaId;
    ++lambdaId;

    // We create a pipe solely for tracking when a child process
    // terminates.  When the child terminates, it will
    // automatically close its end of the pipe, causing a POLLHUP
    // event in the poll() loop.
    int p[2];
    if (::pipe(p) == -1)
        throw syserr("pipe");
    
    if (int pid = ::fork()) {
        // parent process
        ::close(p[1]);
        return { pid, p[0] };
    }
    
    // child process
    ::close(p[0]);
    
    // child process
    // Eigen::IOFormat fmt(Eigen::FullPrecision, Eigen::DontAlignCols, ",", ",", "", "", "", "");
    
    std::string path = "./mpl_lambda_pseudo";

    std::string groupId = std::to_string(pId);

    std::vector<const char*> argv;
    argv.reserve(prob.args().size()+1);

    // argv[0] is the program name
    argv.push_back(path.c_str());

    // need to add the group identifier
    argv.push_back("-I");
    argv.push_back(groupId.c_str());

    std::ostringstream args;
    args << path << " -I " << groupId;
    
    for (auto& arg : prob.args()) {
        argv.push_back(arg.c_str());
        args << ' ' << argv.back();
    }
    argv.push_back(nullptr); // <-- required terminator
    
    char file[20];
    snprintf(file, sizeof(file), "lambda-%04d.out", lambdaId);
    
    // JI_LOG(TRACE) << "RUNNING Lambda: " <<
    //     "./mpl_lambda"

    // for (int i=0 ; argv[i] ; ++i)
    //     args << ' ' << argv[i];
    JI_LOG(TRACE) << "Running lambda:" << args.str();
    
    int fd = ::open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    dup2(fd, 1); // make stdout write to file
    dup2(fd, 2); // make stderr write to file
    close(fd); // close fd, dups remain open
    
    execv(path.c_str(), const_cast<char*const*>(argv.data()));
    
    // if exec returns, then there was a problem
    throw syserr("exec");
}

auto mpl::Coordinator::createGroup(Connection* initiator) -> Group* {
    auto [ it, inserted ] = groups_.emplace(nextGroupId_++, initiator);
    assert(inserted);
    JI_LOG(INFO) << "starting new group " << it->first;
    return &*it;
}

auto mpl::Coordinator::addToGroup(ID id, Connection* conn) -> Group* {
    auto it = groups_.find(id);
    if (it == groups_.end() || it->second.isDone())
        return nullptr;
    
    it->second.connections().push_back(conn);
    return &*it;
}

void mpl::Coordinator::done(Group* group, Connection* conn) {
    auto& connections = group->second.connections();

    // when we get a DONE packet, broadcast DONE to all other
    // connections and mark the group as done.  We broadcast only on
    // the first DONE, but remove the connection from the group in any
    // case.
    for (auto it = connections.begin() ; it != connections.end() ; ) {
        if (*it == conn) {
            it = connections.erase(it);
        } else {
            if (!group->second.isDone())
                (*it)->write(packet::Done(group->first));
            ++it;
        }
    }
    group->second.done();

    // if we're DONE and have no remaining connections, we send DONE
    // to the initiator.
    if (connections.empty()) {
        JI_LOG(INFO) << "no remaining connections in group, sending DONE to initiator";
        group->second.initiator()->write(packet::Done(group->first));
    }

    // if there are no more connections, or the initiator caused the
    // DONE (from a new problem), then we remove the group.
    if (connections.empty() || group->second.initiator() == conn) {
        JI_LOG(INFO) << "removing group " << group->first;
        for (auto it = connections.begin() ; it != connections.end() ; ++it)
            (*it)->degroup();
                
        auto it = groups_.find(group->first);
        if (it != groups_.end())
            groups_.erase(it);
    }
}

template <class T>
void mpl::Coordinator::broadcast(T&& packet, Group* group, Connection* conn) {
    group->second.initiator()->write(packet);
    for (auto* c : group->second.connections())
        if (conn != c)
            c->write(packet);
}

void mpl::Coordinator::launchLambdas(ID groupId, packet::Problem&& prob, int nLambdas) {
    for (int i=0 ; i<nLambdas ; ++i)
        childProcesses_.emplace_back(launchLambda(groupId, prob));
}

void mpl::Coordinator::loop() {
    std::vector<struct pollfd> pfds;

    for (;;) {
        pfds.clear();

        // first comes the fds of the child processes (note that
        // connection processing may change the child process list, so
        // this must be processed first)
        for (auto [ pid, fd ] : childProcesses_) {
            pfds.emplace_back();
            pfds.back().fd = fd;
            pfds.back().events = POLLIN;
        }

        // then et of pollfds is 1:1 with connections
        for (Connection& conn : connections_)
            pfds.emplace_back(conn);

        // then comes the accepting socket
        pfds.emplace_back();
        pfds.back().fd = listen_;
        pfds.back().events = POLLIN;

        JI_LOG(TRACE) << "polling " << pfds.size();
        int nReady = ::poll(pfds.data(), pfds.size(), -1);

        JI_LOG(TRACE) << "poll returned " << nReady;
        
        if (nReady == -1) {
            if (errno == EAGAIN || errno == EINTR)
                continue;
            throw syserr("poll");
        }

        auto pit = pfds.begin();
        for (auto cit = childProcesses_.begin() ; cit != childProcesses_.end() ; ++pit) {
            assert(pit != pfds.end());
            
            if ((pit->revents & POLLHUP) == 0) {
                ++cit;
            } else {
                int stat = 0;
                if (::waitpid(cit->first, &stat, 0) == -1)
                    JI_LOG(WARN) << "waitpid failed with error: " << errno;
                if (::close(cit->second) == -1)
                    JI_LOG(WARN) << "close failed with error: " << errno;
                JI_LOG(INFO) << "child process " << cit->first << " exited with status " << stat;
                
                cit = childProcesses_.erase(cit);
            }
        }

        for (auto cit = connections_.begin() ; cit != connections_.end() ; ++pit) {
            assert(pit != pfds.end());
            
            if (cit->process(*pit))
                ++cit;
            else
                cit = connections_.erase(cit);
        }
        
        assert(pit+1 == pfds.end());
        
        if (pit->revents & (POLLERR | POLLHUP))
            break;
        if (pit->revents & POLLIN) {
            connections_.emplace_back(*this);
            if (!connections_.back()) {
                JI_LOG(WARN) << "accept failed with error: " << errno;
                connections_.pop_back();
            }
            // struct sockaddr_in addr;
            // socklen_t addLen = sizeof(addr);
            
            // int fd = ::accept(listen, &addr, &addrLen);
            // if (fd != -1)
            //     connections.emplace_back(socket_);
            // && !connections.emplace_back(listen_))
            // connections.pop_back();
        }
    }
}


int main(int argc, char *argv[]) {
    mpl::Coordinator coordinator;
    
    coordinator.loop();
}
