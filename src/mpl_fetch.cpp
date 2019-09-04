#include <mpl/demo/fetch_robot.hpp>
#include <iostream>


template <class T, std::size_t I>
T makeState() {
}

template <class T>
struct Make;

template <class S, int rows>
struct Make<Eigen::Matrix<S, rows, 1>> {
    using T = Eigen::Matrix<S, rows, 1>;

    template <std::size_t I, class F, class ... R>
    static void build(T& value, F first, R ... rest) {
        value[I] = first;
        if constexpr (sizeof...(R) > 0)
            build<I+1>(value, rest...);
    }
    
    template <class ... Args>
    static T apply(Args ... args) {
        T value;
        build<0>(value, args...);
        return value;
    }
};

template <class T, class ... S>
void addPath(std::vector<T>& path, S ... args) {
    path.emplace_back(Make<T>::apply(args...));
}

template <class T>
std::vector<T> makePath001() {
    std::vector<T> P;
    addPath(P, 0.38615, 0.508560150174262, 0.375553424018286, -5.94062382778246, 0.214223003680049, 1.29160901715679, -1.72532145407213, -0.987251885861771);
    addPath(P, 0.386142088805617, 0.457014231831145, 0.746881974502196, -3.27722187130138, -0.109209667430568, 1.90544281509359, -1.80977097230234, 0.612925025920071);
    addPath(P, 0.386147010371738, 0.213112583815886, 0.962163198239821, -1.77671577169881, 0.259088727928865, 1.38685795832772, -1.8456573832558, 0.958631445454626);
    addPath(P, 0.386145003700433, 0.614387508495193, 1.38501344006415, -1.37234068808198, 1.73561998803765, 0.765197681841562, -0.391566965391333, 0.268679416206135);
    addPath(P, 0.386142683541026, 1.3021379471638, 0.939420679367169, -1.53199806647382, 2.03464732206441, 0.410895605036528, -0.0902114766771591, 0.628458937297813);
    addPath(P, 0.100000001490116, 1.57079637050629, 1.57079637050629, 0, 1.57079637050629, 0, 1.57079637050629, 0);
    std::reverse(P.begin(), P.end());
    return P;
}

template <class T>
std::vector<T> makePath002() {
    std::vector<T> P;
    addPath(P, 0.38615, 0.801973559455454, 0.414672969328816, 8.16937089205058, -0.606338545452261, 2.91416009655183, 1.19764730707252, 8.41138287434359);
    addPath(P, 0.385897670255518, 0.606077537713378, -0.884090365131937, 6.83217435720473, 0.460270739711006, 1.58860436519146, 1.50392138179204, 7.23765015790164);
    addPath(P, 0.385351817228527, 0.580176091443043, -0.671758754343179, 5.6392728840885, 0.727143258495745, 2.7607723104927, 0.27379945492407, 7.07591190529792);
    addPath(P, 0.380617217689278, 0.434382757123331, -0.151758754343179, 5.3771385914189, 0.352363106849085, 2.39070294374439, 0.603581733939425, 6.8634356478348);
    addPath(P, 0.376185526244643, 0.954382757123331, 0.0969406257579499, 5.03725859880117, -0.162308844441939, 2.00008807823459, 0.630739457442079, 7.12387573008954);
    addPath(P, 0.379870289931012, 0.509945475196034, -0.475042625622348, 3.58685790994216, 0.357780202760977, 1.06318266383589, 0.114201105547848, 5.64915725026278);
    addPath(P, 0.328705751927573, 0.356978412865531, 0.430438764345975, 2.18133426106463, 1.3863671645541, -0.570345858298713, 1.18333133751567, 5.92028963442377);
    addPath(P, 0.301035829454967, 0.420053448466591, 0.23956977580043, 1.89641399726345, 1.06841651361516, -1.09034585829871, 0.747685775699538, 5.70642283235834);
    addPath(P, 0.285227417187641, 0.40507466036898, 0.531369741155522, 0.308098724319502, 1.26045159225192, -0.710792138971174, 1.41116869044652, 4.01578091281487);
    addPath(P, 0.20932923954041, 0.92507466036898, 1.00988566539978, -0.16765833519867, 1.35272063886886, -1.12978450075218, 0.956893544427132, 3.60041342520087);
    addPath(P, 0.195092773862392, 0.957732635601546, 1.02551575501173, -0.332437156969361, 1.29615433598662, -0.609784500752176, 1.08226601953315, 3.12);
    addPath(P, 0.181972674187058, 1.0013256671465, 1.12246167185081, -0.738775518195535, 1.23517282026416, -0.290686451171191, 1.10593880170086, 2.6);
    addPath(P, 0.169935609665969, 1.15206888532727, 1.17249512110764, -0.635522101685855, 1.3480506226988, 0.134795490335083, 1.29195599108338, 2.08);
    addPath(P, 0.154848162398206, 1.31276650293488, 1.28617010793053, -0.549573104627645, 1.40164278821011, 0.58536938378269, 1.42983149844494, 1.56);
    addPath(P, 0.137738537813743, 1.34629610653129, 1.38310915198056, -0.239688476500695, 1.53854534703539, 0.361137629444734, 1.41570658490416, 1.04);
    addPath(P, 0.126078487360453, 1.45769746988383, 1.38586692776312, -0.420527712393827, 1.54619036943838, 0.275651385130523, 1.5195905562347, 0.52);
    addPath(P, 0.100000001490116, 1.57079637050629, 1.57079637050629, 0, 1.57079637050629, 0, 1.57079637050629, 0);
    std::reverse(P.begin(), P.end());
    return P;
}

template <class T>
std::vector<T> makePath003() {
    std::vector<T> P;
    addPath(P, 0.342009765026439, 0.603033629069347, 0.818687809758623, 3.31057461898144, 0.848715586415808, -4.82148658189959, 1.47040524682231, 1.54549869395757);
    addPath(P, 0.13951225411896, 0.177372211371964, 0.669570517828946, 3.16871622633316, 2.01004541446168, -3.55499255270217, 1.20856988888469, 1.83257462777608);
    addPath(P, 0.118429956522941, -0.342627788628036, 0.170802693769544, 3.17717350755562, 1.87710563661761, -3.16746335061948, 1.03917742161299, 2.14931137502374);
    addPath(P, 0.313885665453301, 0.562922266929196, -0.145252703743405, 2.4988729261165, 1.40382031551758, -2.94597753894417, 0.669219822218754, 2.50281901111157);
    addPath(P, 0.24639855138333, 1.13795487970897, 0.424511833042091, 1.86994323893161, 0.893667890715115, -2.26734423959981, 0.915675326829185, 2.28335323526018);
    addPath(P, 0.127289748562299, 0.997175518651635, 0.474512149390198, 1.36311340991091, 0.52005395609246, -1.99322761859398, 0.681872078965692, 2.67465341593641);
    addPath(P, 0.178054805603152, 1.22748137225951, 0.064635415579831, 0.843113409910906, 0.993640978710236, -1.79310541253395, 0.251641571454754, 2.19863258055732);
    addPath(P, 0.208202922038813, 1.11368441113305, -0.119901393232236, -0.152885332368672, 2.06432680267215, -0.774439324365845, 0.9890421038736, 1.1651435866182);
    addPath(P, 0.119794040645698, 1.33871095235714, 0.400098606767764, -0.373629239435118, 1.75305031308584, -0.506757666051433, 1.37533384982841, 0.90522106163889);
    addPath(P, 0.200744649781226, 1.24926751395852, 0.755091164112393, -0.00268870852003267, 1.74339999539584, -0.712255651205343, 0.942508484549711, 0.619291703500511);
    addPath(P, 0.357974887270042, 1.02953832290129, 1.1120673936905, -0.415498501755318, 1.79970946338367, -0.92405834419338, 1.4542944913284, -0.207940361854241);
    addPath(P, 0.106235115125885, 1.37606352480331, 1.39334696175522, -0.420497336506435, 1.59227563078897, -0.52, 1.47382850504935, 0.0509967387614102);
    addPath(P, 0.100000001490116, 1.57079637050629, 1.57079637050629, 0, 1.57079637050629, 0, 1.57079637050629, 0);
    std::reverse(P.begin(), P.end());
    return P;
}

template <class T>
std::vector<T> makePath004() {
    std::vector<T> P;
    addPath(P, 0.100000001490116, 1.57079637050629, 1.57079637050629, 0, 1.57079637050629, 0, 1.57079637050629, 0);
    addPath(P, 0.100855999995356, 1.56386081428139, 1.50130918868797, -0.0989903706641388, 1.39735629221454, 0.52, 1.58879585931846, 0.469645902468689);
    addPath(P, 0.107206185644914, 1.28338152949353, 0.892510163997229, -0.765837013737174, 1.86574350419159, -0.44877681706755, 0.326270618051982, 1.75283563575505);
    addPath(P, 0.146134209731715, 1.06084524887264, 0.222392027684814, -0.559750525236066, 1.15098560960559, 0.0730344384353278, 0.668275152171599, 3.0183393609024);
    addPath(P, 0.235058608873166, 1.33736054206519, -0.524236600126919, 0.924400574052511, 1.50987805634046, 1.53409903918462, -0.654605456783839, 3.86129386875929);
    addPath(P, 0.163980071488252, 1.38335233309347, -0.510434233828327, 2.3306928051568, 0.601333697507475, 0.988657665329117, -0.923307980421795, 4.97068011996075);
    addPath(P, 0.189233885211311, 1.40527828206518, -0.700736184301255, 3.18378135191049, 1.15262069243366, 1.04579559084769, -0.969905302711771, 4.88502150968352);
    addPath(P, 0.277149722361063, 1.28577966983629, -0.270072291401901, 4.5898418271134, 0.480171294074563, 0.948657144040167, -1.63805243708817, 5.41266293611764);
    addPath(P, 0.25899180983652, 1.15577966983629, -0.231947675599641, 5.05253339398005, 0.959634841966891, 1.41421331456856, -1.44451275918464, 5.15698503837647);
    addPath(P, 0.300967412754726, 1.25338125299163, -0.186586028854764, 5.57253339398005, 1.3890311688512, 1.89880950691693, -1.64098513300362, 5.48883763041989);
    addPath(P, 0.237053415787138, 0.922679195227677, -0.118863649299473, 7.1750493450317, -0.0960593863224897, 1.63598141244465, -1.04834081294609, 6.8896495764381);
    addPath(P, 0.227051188437575, 0.848596844509275, -0.362179007088691, 7.67643900717424, 0.367679174404923, 1.17737457534905, -0.972379400873856, 6.95225068550396);
    addPath(P, 0.306060476361402, 0.911923103773163, -0.5760066540028, 9.48326674222084, 0.298013161511595, 0.382949195226512, 0.73167549114718, 8.63223513784014);
    addPath(P, 0.223440921440245, 0.978609512990633, 0.217597568587497, 10.9938565425021, 1.07903312809762, 0.131680590816308, 0.881590136030784, 7.99893983937344);
    addPath(P, 0.252623040256979, 1.04073137409617, 0.270434308594699, 11.1329881657378, 1.26892179513954, -0.182549127206505, 0.478637426623609, 8.30064120868406);
    return P;
}

template <class T>
std::vector<T> makePath005() {
    std::vector<T> P;
    addPath(P, 0.100000001490116, 1.57079637050629, 1.57079637050629, 0, 1.57079637050629, 0, 1.57079637050629, 0);
    addPath(P, 0.10050809571988, 1.56167672868245, 1.4456491790416, -0.338504721018852, 1.52998969600723, 0.52, 1.52261219301189, -0.456589516769067);
    addPath(P, 0.137808715351205, 1.48443659179324, 1.14731997103924, -1.09683854369333, 1.24408687921949, 0.0188591414845676, 0.996971283946687, -2.07417119505082);
    addPath(P, 0.15343613687481, 1.28299673446916, 0.806207752094414, -1.60017352640475, 1.21579283117997, -1.20721760478341, 0.20943940448404, -3.26145892484869);
    addPath(P, 0.164708583769052, 1.09926241413529, -0.0390536852579469, 0.25779423904114, 0.783316918913523, -0.240049670120353, 0.588029886283305, -5.51513201028812);
    addPath(P, 0.208445266086966, 0.935962514756116, 0.448035589816767, 1.40477737922811, 0.992086069009173, -1.4632075259255, 1.06582144417312, -6.62750212129193);
    addPath(P, 0.0965560779423526, 1.3095877905756, -0.115364619148821, 2.83344826288873, -0.398914407947131, -1.75067603385537, 0.82606035639922, -8.29778410368656);
    addPath(P, 0.196371156326255, 1.43627516919909, -0.773797850614854, 4.31549339866526, -0.251749807172729, -3.1302539694753, 0.311851326476815, -8.62126527344623);
    addPath(P, 0.213388468157019, 0.983256967019503, 0.234082792853115, 6.609624241979, 1.22352541819612, -4.9547507413007, 0.629464314106492, -10.6888766162134);
    addPath(P, 0.243321532316925, 0.964674310931273, 0.394499480238181, 7.12654086656004, 1.44274256858205, -5.4747507413007, 0.409451143468645, -10.3092422895921);
    addPath(P, 0.287819673368357, 0.838716507433652, 0.644525170216493, 7.64654086656004, 1.47034188893457, -5.4443869589002, 0.895249225485539, -10.0090807148311);
    addPath(P, 0.253283568132652, 0.932410939786543, 0.719437874808684, 7.86724798873821, 1.76422862511655, -5.82259839808619, 0.960952279406101, -9.96208088547577);
    addPath(P, 0.313888114681079, 0.997825652027538, 0.97808975914443, 9.13971133004989, 1.21918081657167, -4.79231646748887, 1.52161904502027, -10.926194468624);
    addPath(P, 0.336907995889814, 0.93891151057762, 1.23808975914443, 9.47213352519577, 1.57275282834299, -5.0291287110026, 1.76040059119609, -11.2667037406803);
    addPath(P, 0.335139951610205, 0.81051818072787, 0.837560028320769, 10.1104921741094, 1.02472040860885, -5.11885633773712, 1.08566536612403, -11.0680352787079);
    return P;
}

int main(int argc, char *argv[]) {
    using S = double;
    using namespace mpl::demo;
    using Robot = FetchRobot<S>;
    using Config = typename Robot::Config;

    std::mt19937_64 rng;

    typename Robot::Frame envFrame;
    envFrame.setIdentity();
    envFrame.translation() << 0.57, -0.90, 0;
    Eigen::AngleAxis<S> envAngle(-1.570796326794897, Eigen::Matrix<S,3,1>::UnitZ());
    envFrame.linear() = envAngle.toRotationMatrix();
    Eigen::IOFormat fmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ",", ",");
    
    BlenderPy bpy(std::cout);
    bpy << "#!/usr/bin/env blender --python";
    bpy << "import bpy";
    bpy << "bpy.ops.object.delete(use_global=False)";

    bpy << "Env = bpy.data.objects.new(\"Env\", None)";
    bpy << "bpy.context.collection.objects.link(Env)";
    bpy << "bpy.ops.wm.collada_import(filepath=\"../../resources/AUTOLAB.dae\")";
    bpy << "for obj in bpy.context.selected_objects:";
    bpy << "    obj.constraints.new(type='CHILD_OF')";
    bpy << "    obj.constraints['Child Of'].target = Env";
    bpy << "Env.location = (" << envFrame.translation().format(fmt) << ")";
    bpy << "Env.rotation_mode = 'AXIS_ANGLE'";
    bpy << "Env.rotation_axis_angle = (" << envAngle.angle() << ',' << envAngle.axis().format(fmt) << ")";
    
    Robot robot;
    robot.toArticulatedBlenderScript(bpy, "../../resources/fetch/");

    robot.setConfig(Robot::Config::Zero()); // restConfig());
    assert(!robot.selfCollision());


    // envFrame = envFrame.inverse();

    // should be around 0.82, 0.53, 0.88
    typename Robot::Frame ikTarget;
    ikTarget.setIdentity();
    ikTarget.translation() << -1.07,0.16,0.88;

    // std::clog << "ikTarget\n" << ikTarget.matrix() << "\n"
    //           << "env * ikTarget\n"    << (envFrame*ikTarget).matrix() << "\n"
    //           << "env^-1 * ikTarget\n" << (envFrame.inverse()*ikTarget).matrix() << "\n"
    //           << "ikTarget * env\n"    << (ikTarget * envFrame).matrix() << "\n"
    //           << "ikTarget * env^-1\n" << (ikTarget * envFrame.inverse()).matrix() << "\n";
    
    ikTarget = envFrame * ikTarget;
    // ikTarget.translation() -= envFrame.translation();
    
    std::clog << "IK Target:\n" << ikTarget.matrix() << std::endl;

    std::clog << "Gripper Axis:\n" << robot.gripperAxis().matrix() << std::endl;
    
    // Eigen::Matrix<S, 6, 1> L;
    // L << 1, 1, 1,   1, 1, 0.00001; // /1.570796326794897;

    // L << 0.01, 0.01, 0.01, 0.01, 0.01, 1.570796326794897;
    // S eps = L.minCoeff();
    // L = eps / L.array();

    // for (int iter = 0 ; iter<100 ; ++iter) {
    //     robot.setConfig(Robot::randomConfig(rng));
    //     if (robot.ik(ikTarget, L, eps)) {
    //         std::clog << "IK SOLVED after " << iter << std::endl;
    //         break;
    //     }
    // }
    
    // robot.updateArticulatedBlenderScript(bpy);

    std::vector<Config> path = makePath005<Config>();
        
    // robot.toCollisionGeometryBlenderScript(bpy);

    for (std::size_t i=0 ; i<path.size() ; ++i) {
        robot.setConfig(path[i]);
        robot.updateArticulatedBlenderScript(bpy);
        robot.keyframeInsert(bpy, i*20 + 1);
    }
    
    // for (int frame=0 ; frame<1 ; ++frame) {
    //     do {
    //         robot.setConfig(Robot::randomConfig(rng));
    //     } while (robot.selfCollision());

    //     robot.updateArticulatedBlenderScript(bpy);
    //     robot.keyframeInsert(bpy, frame*20 + 1);
    // }
    
    return EXIT_SUCCESS;
}
