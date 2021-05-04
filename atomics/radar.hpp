/**
* Radar atomic model
*/

#ifndef _RADAR_HPP__
#define _RADAR_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <iomanip>
#include <cmath>

#include "../data_structures/message.hpp"
#include "../usrlib/engagementUtils.hpp"

#define TORAD(X) (X)*(3.1415926535/180)
#define TODEG(X) (X)*(180/3.1415926535)
#define RADIUS_EARTH 6373000

using namespace cadmium;
using namespace std;

// Port definition
struct Radar_defs {
    struct inAircraftPosition : public in_port<messageAircraftPosition> {
    };
    struct outDetectedPosition : public out_port<messageDetectedPosition> {
    };
};

// Atomic model class
template<typename TIME>
class Radar {

    enum phase {
        SEARCHING, TRACKING
    };

public:

    // ports definition
    using input_ports = tuple<typename Radar_defs::inAircraftPosition>;
    using output_ports = tuple<typename Radar_defs::outDetectedPosition>;

    // state definition
    struct state_type {
        TIME sigma;
        long double aircraftLongitude;
        long double aircraftLatitude;
        phase currPhase;
        double distanceRadarAircraft;
        long double detectedAircraftLatitude;
        long double detectedAircraftLongitude;
        long double radarLatitude;
        long double radarLongitude;
        double radarRange;
    };

    state_type state;

    // default constructor
    Radar() {
        state.sigma = std::numeric_limits<TIME>::infinity();
        state.detectedAircraftLatitude = 0;
        state.detectedAircraftLongitude = 0;
        state.currPhase = SEARCHING;
        state.radarLatitude = SELECTED_RADAR_POSITION_LAT;
        state.radarLongitude = SELECTED_RADAR_POSITION_LONG;
        state.radarRange = SELECTED_RADAR_RANGE;
    }

    // internal transition
    void internal_transition() {

        switch (state.currPhase) {
            case TRACKING:
                state.currPhase = SEARCHING;
                state.sigma = std::numeric_limits<TIME>::infinity();
                break;
        }
    }

    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {

        for (const auto &x : get_messages<typename Radar_defs::inAircraftPosition>(mbs)) {

            state.detectedAircraftLatitude = x.aircraftLatitude;
            state.detectedAircraftLongitude = x.aircraftLongitude;
            // Calculate distance between radar and aircraft
            state.distanceRadarAircraft = CalculateDistance(state.radarLatitude, state.radarLongitude,
                                                            state.detectedAircraftLatitude,
                                                            state.detectedAircraftLongitude);

            if (state.distanceRadarAircraft <= state.radarRange) {
                state.currPhase = TRACKING;
                state.sigma = TIME("00:00:01.000");
            } else {
                state.currPhase = SEARCHING;
                state.sigma = std::numeric_limits<TIME>::infinity();
            }
        }
    }

    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }

    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;

        if (state.currPhase == TRACKING) {
            get_messages<typename Radar_defs::outDetectedPosition>(bags).push_back(
                    messageDetectedPosition(state.radarRange, state.radarLatitude, state.radarLongitude,
                                            state.detectedAircraftLatitude, state.detectedAircraftLongitude));
        }
        return bags;
    }

    // time_advance function
    TIME time_advance() const {
        return state.sigma;
    }

    friend ostringstream &operator<<(ostringstream &os, const typename Radar<TIME>::state_type &currState) {

        switch (currState.currPhase) {

            case SEARCHING:
                os << "SEARCHING - Distance between the radar and the aircraft is: " << currState.distanceRadarAircraft
                   << " meters.";
                break;

            case TRACKING:
                os << "TRACKING - Distance between the radar and the aircraft is: " << currState.distanceRadarAircraft
                   << " meters.";
                break;
        }

        return os;

    }
};

#endif  // _RADAR_HPP__