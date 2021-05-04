//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>

//Time class header
#include <NDTime.hpp>

//Messages structures
#include "../data_structures/message.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/gunner.hpp"

//C++ libraries
#include <string>
#include <iostream>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
/************************************************/
struct inOrder : public cadmium::in_port<messageOrder>{ };

/***** Define output ports for coupled model *****/
/*************************************************/
struct outDistanceGunnerAircraft : public out_port<messageDistance> {};
struct outGunnerStatus : public out_port<messageGunnerStatus> {};

/****** Input Reader atomic model declaration ******/
/***************************************************/
template<typename T>
class InputReader_messageOrder : public iestream_input<messageOrder,T> {
    public:
        InputReader_messageOrder() = default;
        InputReader_messageOrder(const char* file_path) : iestream_input<messageOrder,T>(file_path) {}
};

int main(int argc, char *argv[]){

    if (argc == 2) {
        ReadOptions(argv[1]);
    }

    /****** Input Reader atomic model instantiation ******/
    /*****************************************************/
    const char * i_input_data_order = "../input_data/gunner_order_input_test _t2.txt";
    shared_ptr<dynamic::modeling::model> input_reader_order;
    input_reader_order = dynamic::translate::make_dynamic_atomic_model<InputReader_messageOrder, TIME, const char*>("input_reader_order", move(i_input_data_order));

    /****** Gunner atomic model instantiation ******/
    /***********************************************/
    shared_ptr<dynamic::modeling::model> gunner1;
    gunner1 = dynamic::translate::make_dynamic_atomic_model<Gunner, TIME, int>("gunner1",1);

    shared_ptr<dynamic::modeling::model> gunner2;
    gunner2 = dynamic::translate::make_dynamic_atomic_model<Gunner, TIME, int>("gunner2",2);

    shared_ptr<dynamic::modeling::model> gunner3;
    gunner3 = dynamic::translate::make_dynamic_atomic_model<Gunner, TIME, int>("gunner3",3);

    /*******TOP MODEL*******/
    /***********************/
    dynamic::modeling::Ports iports_TOP;
    iports_TOP = {};

    dynamic::modeling::Ports oports_TOP;
    oports_TOP = {typeid(outGunnerStatus),typeid(outDistanceGunnerAircraft)};

    dynamic::modeling::Models submodels_TOP;
    submodels_TOP = {input_reader_order, gunner1, gunner2, gunner3};

    dynamic::modeling::EICs eics_TOP;
    eics_TOP = {};

    dynamic::modeling::EOCs eocs_TOP;
    eocs_TOP = {
        dynamic::translate::make_EOC<Gunner_defs::outDistanceGunnerAircraft,outDistanceGunnerAircraft>("gunner1"),
        dynamic::translate::make_EOC<Gunner_defs::outGunnerStatus,outGunnerStatus>("gunner1"),
        dynamic::translate::make_EOC<Gunner_defs::outDistanceGunnerAircraft,outDistanceGunnerAircraft>("gunner2"),
        dynamic::translate::make_EOC<Gunner_defs::outGunnerStatus,outGunnerStatus>("gunner2"),
        dynamic::translate::make_EOC<Gunner_defs::outDistanceGunnerAircraft,outDistanceGunnerAircraft>("gunner3"),
        dynamic::translate::make_EOC<Gunner_defs::outGunnerStatus,outGunnerStatus>("gunner3")
    };

    dynamic::modeling::ICs ics_TOP;
    ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<messageOrder>::out,Gunner_defs::inOrder>("input_reader_order","gunner1"),
        dynamic::translate::make_IC<iestream_input_defs<messageOrder>::out,Gunner_defs::inOrder>("input_reader_order","gunner2"),
        dynamic::translate::make_IC<iestream_input_defs<messageOrder>::out,Gunner_defs::inOrder>("input_reader_order","gunner3")
        };

    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );

    /*************** Loggers *******************/
    /*******************************************/
    static ofstream out_messages("../simulation_results/gunner_test_output_messages_t2.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };

    static ofstream out_state("../simulation_results/gunner_test_output_state_t2.txt");
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

    /************** Runner call **************/
    /*****************************************/
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until(NDTime("00:20:00:000"));

    return 0;

}
