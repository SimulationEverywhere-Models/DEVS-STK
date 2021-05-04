/**
* Command and Control (C2) atomic model
*/

#ifndef __C2_HPP__
#define __C2_HPP__

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
struct C2_defs {
    struct inDetectedPosition : public in_port<messageDetectedPosition> {
    };
    struct inMissileStatus : public in_port<messageMissileStatus> {
    };
    struct inGunnerStatus : public in_port<messageGunnerStatus> {
    };
    struct outOrder : public out_port<messageOrder> {
    };
};

// Atomic model class
template<typename TIME>
class C2 {

    enum phase {
        WAITING, DECISION_MAKING, ORDERING, STANDING_DOWN
    };

public:

    // ports definition
    using input_ports = tuple<typename C2_defs::inDetectedPosition, typename C2_defs::inMissileStatus, typename C2_defs::inGunnerStatus>;
    using output_ports = tuple<typename C2_defs::outOrder>;

    // state definition
    struct state_type {
        TIME sigma;
        phase currPhase;
        long double aircraftLongitude;
        long double aircraftLatitude;
        double distanceGunnerAircraft;
        long double detectedAircraftLatitude;
        long double detectedAircraftLongitude;
        long double gunnerLatitude;
        long double gunnerLongitude;
        double gunnerRange;
        int gunnerID;
        long double C2Latitude;
        long double C2Longitude;
    };

    state_type state;

    // default constructor
    C2() {
        state.currPhase = WAITING;
        state.sigma = std::numeric_limits<TIME>::infinity();
        state.gunnerRange = 6000;
        state.C2Latitude = SELECTED_C2_POSITION_LAT;
        state.C2Longitude = SELECTED_C2_POSITION_LONG;
    }

    // internal transition
    void internal_transition() {

        switch (state.currPhase) {
            case ORDERING:
                state.currPhase = WAITING;
                state.sigma = std::numeric_limits<TIME>::infinity();
                break;
        }
    }

    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {

        for (const auto &y : get_messages<typename C2_defs::inGunnerStatus>(mbs)) {

            if (state.currPhase == DECISION_MAKING) {

                state.gunnerID = y.gunnerID;
                state.gunnerLatitude = SELECTED_GUNNERS_POSITION_LAT[state.gunnerID - 1];
                state.gunnerLongitude = SELECTED_GUNNERS_POSITION_LONG[state.gunnerID - 1];
                state.distanceGunnerAircraft = CalculateDistance(state.gunnerLatitude, state.gunnerLongitude,
                                                                 state.detectedAircraftLatitude,
                                                                 state.detectedAircraftLongitude);

                if (state.distanceGunnerAircraft <= state.gunnerRange) {
                    state.currPhase = ORDERING;
                    state.sigma = TIME("00:00:00");
                } else {
                    state.currPhase = DECISION_MAKING;
                    state.sigma = std::numeric_limits<TIME>::infinity();
                }
            }
        }

        for (const auto &z : get_messages<typename C2_defs::inMissileStatus>(mbs)) {
            if (z.missileStatus == 0) {
                state.currPhase = WAITING;
                state.sigma = std::numeric_limits<TIME>::infinity();
            } else {
                state.currPhase = STANDING_DOWN;
                state.sigma = std::numeric_limits<TIME>::infinity();
            }

        }

        for (const auto &x : get_messages<typename C2_defs::inDetectedPosition>(mbs)) {
            state.detectedAircraftLongitude = x.detectedAircraftLongitude;
            state.detectedAircraftLatitude = x.detectedAircraftLatitude;
            if (state.currPhase == WAITING) {
                state.currPhase = DECISION_MAKING;
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

        if (state.currPhase == ORDERING) {
            get_messages<typename C2_defs::outOrder>(bags).push_back(
                    messageOrder(state.gunnerID, state.distanceGunnerAircraft, state.C2Latitude, state.C2Longitude));
        }

        return bags;
    }


// time_advance function
    TIME time_advance() const {
        return state.sigma;
    }

    friend ostringstream &operator<<(ostringstream &os, const typename C2<TIME>::state_type &currState) {

        switch (currState.currPhase) {

            case WAITING:
                os << "WAITING";
                break;

            case STANDING_DOWN:
                os << "STANDING DOWN - Aircraft has been hit.";
                break;

            case DECISION_MAKING:
                os << "MAKING A DECISION";
                break;

            case ORDERING:
                os << "ORDERING";
                break;
        }
        return os;
    }
};

#endif  // __C2_HPP__