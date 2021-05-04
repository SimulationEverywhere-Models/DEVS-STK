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
#include "../atomics/c2.hpp"

//C++ libraries
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
/************************************************/

/***** Define output ports for coupled model *****/
/*************************************************/
struct outOrder : public out_port<messageOrder> {};

/****** Input Reader atomic model declaration ******/
/***************************************************/
template<typename T>
class InputReader_messageDetectedPosition : public iestream_input<messageDetectedPosition,T> {
    public:
        InputReader_messageDetectedPosition() = default;
        InputReader_messageDetectedPosition(const char* file_path) : iestream_input<messageDetectedPosition,T>(file_path) {}
};

template<typename T>
class InputReader_messageMissileStatus : public iestream_input<messageMissileStatus,T> {
public:
    InputReader_messageMissileStatus() = default;
    InputReader_messageMissileStatus(const char* file_path) : iestream_input<messageMissileStatus,T>(file_path) {}
};

template<typename T>
class InputReader_messageGunnerStatus : public iestream_input<messageGunnerStatus,T> {
public:
    InputReader_messageGunnerStatus() = default;
    InputReader_messageGunnerStatus(const char* file_path) : iestream_input<messageGunnerStatus,T>(file_path) {}
};

int main(int argc, char *argv[]){

    if (argc == 2) {
        ReadOptions(argv[1]);
    }

    /****** Input Reader atomic model instantiation ******/
    /*****************************************************/
    const char * i_input_data_detected_position = "../input_data/c2_detected_position_input_test_t6.txt";
    shared_ptr<dynamic::modeling::model> input_reader_detected_position;
    input_reader_detected_position = dynamic::translate::make_dynamic_atomic_model<InputReader_messageDetectedPosition, TIME, const char*>("input_reader_detected_position", move(i_input_data_detected_position));

    const char * i_input_data_missile_status = "../input_data/c2_missile_status_input_test_t6.txt";
    shared_ptr<dynamic::modeling::model> input_reader_missile_status;
    input_reader_missile_status = dynamic::translate::make_dynamic_atomic_model<InputReader_messageMissileStatus, TIME, const char*>("input_reader_missile_status", move(i_input_data_missile_status));

    const char * i_input_data_gunner_status = "../input_data/c2_gunner_status_input_test_t6.txt";
    shared_ptr<dynamic::modeling::model> input_reader_gunner_status;
    input_reader_gunner_status = dynamic::translate::make_dynamic_atomic_model<InputReader_messageGunnerStatus, TIME, const char*>("input_reader_gunner_status", move(i_input_data_gunner_status));

    /****** C2 atomic model instantiation ******/
    /***********************************************/
    shared_ptr<dynamic::modeling::model> c2;
    c2 = dynamic::translate::make_dynamic_atomic_model<C2, TIME>("c2");

    /*******TOP MODEL*******/
    /***********************/
    dynamic::modeling::Ports iports_TOP;
    iports_TOP = {};

    dynamic::modeling::Ports oports_TOP;
    oports_TOP = {typeid(outOrder)};

    dynamic::modeling::Models submodels_TOP;
    submodels_TOP = {input_reader_detected_position,input_reader_missile_status,input_reader_gunner_status,c2};

    dynamic::modeling::EICs eics_TOP;
    eics_TOP = {};

    dynamic::modeling::EOCs eocs_TOP;
    eocs_TOP = {
        dynamic::translate::make_EOC<C2_defs::outOrder,outOrder>("c2")
    };

    dynamic::modeling::ICs ics_TOP;
    ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<messageDetectedPosition>::out,C2_defs::inDetectedPosition>("input_reader_detected_position","c2"),
        dynamic::translate::make_IC<iestream_input_defs<messageMissileStatus>::out,C2_defs::inMissileStatus>("input_reader_missile_status","c2"),
        dynamic::translate::make_IC<iestream_input_defs<messageGunnerStatus>::out,C2_defs::inGunnerStatus>("input_reader_gunner_status","c2")
        };

    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );

    /*************** Loggers *******************/
    /*******************************************/
    static ofstream out_messages("../simulation_results/c2_test_output_messages_t6.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };

    static ofstream out_state("../simulation_results/c2_test_output_state_t6.txt");
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
