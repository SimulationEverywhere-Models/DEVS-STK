#ifndef __ENGAGEMENTUTILS_CPP__
#define __ENGAGEMENTUTILS_CPP__

//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Json exporter header
#include <dynamic_json_exporter.hpp>

//Time class header
#include <NDTime.hpp>

//Messages structures
#include "../data_structures/message.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/gunner.hpp"
#include "../atomics/missile.hpp"
#include "../atomics/aircraft.hpp"
#include "../atomics/radar.hpp"
#include "../atomics/c2.hpp"

//C++ headers
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
/************************************************/

// Air Defense System (ADS) Coupled Model
struct ads_in: public in_port<messageAircraftPosition>{};

// Radar_Augmented MANPADS Engagement Coupled Model
// N/A

/***** Define output ports for coupled models *****/
/**************************************************/

// Air Defense System (ADS) Coupled Model
struct ads_out: public out_port<messageMissileStatus>{};

// Radar_Augmented MANPADS Engagement Coupled Model
struct radar_MANPADS_engagement_out: public out_port<messageMissileStatus>{};

int main(int argc, char *argv[]){

    if (argc == 2) {
        ReadOptions(argv[1]);
    }

    /****** Atomic model instantiation *******************/
    /*****************************************************/
    shared_ptr<dynamic::modeling::model> radar;
    radar = dynamic::translate::make_dynamic_atomic_model<Radar, TIME>("radar");

    shared_ptr<dynamic::modeling::model> c2;
    c2 = dynamic::translate::make_dynamic_atomic_model<C2, TIME>("c2");

    shared_ptr<dynamic::modeling::model> missile1;
    missile1 = dynamic::translate::make_dynamic_atomic_model<Missile, TIME, int>("missile1",1);

    shared_ptr<dynamic::modeling::model> missile2;
    missile2 = dynamic::translate::make_dynamic_atomic_model<Missile, TIME, int>("missile2",2);

    shared_ptr<dynamic::modeling::model> missile3;
    missile3 = dynamic::translate::make_dynamic_atomic_model<Missile, TIME, int>("missile3",3);

    shared_ptr<dynamic::modeling::model> gunner1;
    gunner1 = dynamic::translate::make_dynamic_atomic_model<Gunner, TIME, int>("gunner1",1);

    shared_ptr<dynamic::modeling::model> gunner2;
    gunner2 = dynamic::translate::make_dynamic_atomic_model<Gunner, TIME, int>("gunner2",2);

    shared_ptr<dynamic::modeling::model> gunner3;
    gunner3 = dynamic::translate::make_dynamic_atomic_model<Gunner, TIME, int>("gunner3",3);

    shared_ptr<dynamic::modeling::model> aircraft;
    aircraft = dynamic::translate::make_dynamic_atomic_model<Aircraft, TIME>("aircraft");

    /******* Air Defense System (ADS) Coupled MODEL********/
    /******************************************************/
    dynamic::modeling::Ports iports_ADS;
    iports_ADS = {typeid(ads_in)};

    dynamic::modeling::Ports oports_ADS;
    oports_ADS = {typeid(ads_out)};

    dynamic::modeling::Models submodels_ADS;
    submodels_ADS = {radar, c2, gunner1, gunner2, gunner3, missile1, missile2, missile3};

    dynamic::modeling::EICs eics_ADS;
    eics_ADS = {
            dynamic::translate::make_EIC<ads_in,Radar_defs::inAircraftPosition>("radar")
    };

    dynamic::modeling::EOCs eocs_ADS;
    eocs_ADS = {
            dynamic::translate::make_EOC<Missile_defs::outMissileStatus,ads_out>("missile1"),
            dynamic::translate::make_EOC<Missile_defs::outMissileStatus,ads_out>("missile2"),
            dynamic::translate::make_EOC<Missile_defs::outMissileStatus,ads_out>("missile3")
    };

    dynamic::modeling::ICs ics_ADS;
    ics_ADS = {
            dynamic::translate::make_IC<Radar_defs::outDetectedPosition,C2_defs::inDetectedPosition>("radar","c2"),
            dynamic::translate::make_IC<C2_defs::outOrder,Gunner_defs::inOrder>("c2","gunner1"),
            dynamic::translate::make_IC<C2_defs::outOrder,Gunner_defs::inOrder>("c2","gunner2"),
            dynamic::translate::make_IC<C2_defs::outOrder,Gunner_defs::inOrder>("c2","gunner3"),
            dynamic::translate::make_IC<Gunner_defs::outDistanceGunnerAircraft,Missile_defs::inDistanceGunnerAircraft>("gunner1","missile1"),
            dynamic::translate::make_IC<Gunner_defs::outDistanceGunnerAircraft,Missile_defs::inDistanceGunnerAircraft>("gunner2","missile2"),
            dynamic::translate::make_IC<Gunner_defs::outDistanceGunnerAircraft,Missile_defs::inDistanceGunnerAircraft>("gunner3","missile3"),
            dynamic::translate::make_IC<Gunner_defs::outGunnerStatus,C2_defs::inGunnerStatus>("gunner1","c2"),
            dynamic::translate::make_IC<Gunner_defs::outGunnerStatus,C2_defs::inGunnerStatus>("gunner2","c2"),
            dynamic::translate::make_IC<Gunner_defs::outGunnerStatus,C2_defs::inGunnerStatus>("gunner3","c2"),
            dynamic::translate::make_IC<Missile_defs::outMissileStatus,C2_defs::inMissileStatus>("missile1","c2"),
            dynamic::translate::make_IC<Missile_defs::outMissileStatus,C2_defs::inMissileStatus>("missile2","c2"),
            dynamic::translate::make_IC<Missile_defs::outMissileStatus,C2_defs::inMissileStatus>("missile3","c2")
    };

    shared_ptr<dynamic::modeling::coupled<TIME>> ADS_MODEL;
    ADS_MODEL = make_shared<dynamic::modeling::coupled<TIME>>(
            "ADS_Model", submodels_ADS, iports_ADS, oports_ADS, eics_ADS, eocs_ADS, ics_ADS
    );

    /******* Radar_Augmented MANPADS Engagement Coupled MODEL********/
    /****************************************************************/
    dynamic::modeling::Ports iports_TOP;
    iports_TOP = {};

    dynamic::modeling::Ports oports_TOP;
    oports_TOP = {typeid(radar_MANPADS_engagement_out)};

    dynamic::modeling::Models submodels_TOP;
    submodels_TOP = {aircraft, ADS_MODEL};

    dynamic::modeling::EICs eics_TOP;
    eics_TOP = {};

    dynamic::modeling::EOCs eocs_TOP;
    eocs_TOP = {
            dynamic::translate::make_EOC<ads_out,radar_MANPADS_engagement_out>("ADS_Model")
    };

    dynamic::modeling::ICs ics_TOP;
    ics_TOP = {
            dynamic::translate::make_IC<Aircraft_defs::outAircraftPosition, ads_in>("aircraft","ADS_Model"),
            dynamic::translate::make_IC<ads_out, Aircraft_defs::inMissileStatus>("ADS_Model","aircraft")
    };

    shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
            "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
    );

    /*************** Loggers *******************/
    /*******************************************/
    static ofstream out_messages("../simulation_results/radar_MANPADS_engagement_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){
            return out_messages;
        }
    };

    static ofstream out_state("../simulation_results/radar_MANPADS_engagement_output_state.txt");
    struct oss_sink_state{
        static ostream& sink(){
            return out_state;
        }
    };

    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;

    /************** Runner call ************************/
    /***************************************************/
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until(NDTime("00:15:00:000"));

    /************** JSON Exporter call ************************/
    /**********************************************************/
    //static ofstream out_JSON("radar_MANPADS_engagement_json.json");
	//dynamic_export_model_to_json(out_JSON, TOP);

    return 0;
}

#endif  // __ENGAGEMENTUTILS_CPP__