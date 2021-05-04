/**
* Aircraft atomic model
*/

#ifndef _AIRCRAFT_HPP__
#define _AIRCRAFT_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <iostream>
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
struct Aircraft_defs {
    struct inMissileStatus : public in_port<messageMissileStatus> {
    };
    struct outAircraftPosition : public out_port<messageAircraftPosition> {
    };
};

// Atomic model class
template<typename TIME>
class Aircraft {

    enum phase {
        FLYING, MISSION_COMPLETED, HIT
    };

public:

    // ports definition
    using input_ports = tuple<typename Aircraft_defs::inMissileStatus>;
    using output_ports = tuple<typename Aircraft_defs::outAircraftPosition>;

    // state definition
    struct state_type {
        TIME sigma;
        phase currPhase;
        long double aircraftPositionLat;
        long double aircraftPositionLong;
        long double aircraftInitialPositionLat;
        long double aircraftInitialPositionLong;
        long double aircraftFinalPositionLat;
        long double aircraftFinalPositionLong;
        double aircraftSpeed;
        double aircraftAltitude;
        long double distanceFlown;
        int missileStatus;
        long double flightPathDistance;
        long double aircraftBearing;
    };
    state_type state;

    // default constructor
    Aircraft() {
        state.aircraftPositionLat = SELECTED_AIRCRAFT_INITIAL_POSITION_LAT;
        state.aircraftPositionLong = SELECTED_AIRCRAFT_INITIAL_POSITION_LONG;
        state.aircraftInitialPositionLat = SELECTED_AIRCRAFT_INITIAL_POSITION_LAT;
        state.aircraftInitialPositionLong = SELECTED_AIRCRAFT_INITIAL_POSITION_LONG;
        state.aircraftFinalPositionLat = SELECTED_AIRCRAFT_FINAL_POSITION_LAT;
        state.aircraftFinalPositionLong = SELECTED_AIRCRAFT_FINAL_POSITION_LONG;
        state.aircraftSpeed = SELECTED_AIRCRAFT_SPEED;
        state.aircraftAltitude = SELECTED_AIRCRAFT_ALTITUDE;
        state.flightPathDistance = CalculateDistance(SELECTED_AIRCRAFT_INITIAL_POSITION_LAT,
                                                     SELECTED_AIRCRAFT_INITIAL_POSITION_LONG,
                                                     SELECTED_AIRCRAFT_FINAL_POSITION_LAT,
                                                     SELECTED_AIRCRAFT_FINAL_POSITION_LONG);
        state.aircraftBearing = CalculateBearing(SELECTED_AIRCRAFT_INITIAL_POSITION_LAT,
                                                 SELECTED_AIRCRAFT_INITIAL_POSITION_LONG,
                                                 SELECTED_AIRCRAFT_FINAL_POSITION_LAT,
                                                 SELECTED_AIRCRAFT_FINAL_POSITION_LONG);
        state.distanceFlown = 0;
        state.currPhase = FLYING;
        state.sigma = TIME();
    }

    // internal transition
    void internal_transition() {

        // Calculate new aircraft position
        state.aircraftPositionLat = TODEG(asin(
                sin((TORAD(state.aircraftPositionLat))) * cos((state.aircraftSpeed / RADIUS_EARTH)) +
                cos((TORAD(state.aircraftPositionLat))) * sin((state.aircraftSpeed / RADIUS_EARTH)) *
                cos((TORAD(state.aircraftBearing)))));
        state.aircraftPositionLong = TODEG((TORAD(state.aircraftPositionLong)) +
                                           atan2(sin((TORAD(state.aircraftBearing))) *
                                                 sin((state.aircraftSpeed / RADIUS_EARTH)) *
                                                 cos((TORAD(state.aircraftPositionLat))),
                                                 cos((state.aircraftSpeed / RADIUS_EARTH)) -
                                                 sin((TORAD(state.aircraftPositionLat))) *
                                                 sin((TORAD(state.aircraftPositionLat)))));
        state.aircraftBearing = CalculateBearing(state.aircraftPositionLat, state.aircraftPositionLong,
                                                 SELECTED_AIRCRAFT_FINAL_POSITION_LAT,
                                                 SELECTED_AIRCRAFT_FINAL_POSITION_LONG);

        // Calculate distance flown
        state.distanceFlown = CalculateDistance(state.aircraftInitialPositionLat, state.aircraftInitialPositionLong,
                                                state.aircraftPositionLat, state.aircraftPositionLong);

        // Verify if aircraft has reached the end of its predefined path
        if (state.distanceFlown >= state.flightPathDistance) {
            state.sigma = std::numeric_limits<TIME>::infinity();
            state.currPhase = MISSION_COMPLETED;
        } else {
            state.sigma = TIME("00:00:01.000");
            state.currPhase = FLYING;
        }
    }

    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {

        for (const auto &x : get_messages<typename Aircraft_defs::inMissileStatus>(mbs)) {

            if (state.distanceFlown < state.flightPathDistance) {
                state.missileStatus = x.missileStatus;
                if (state.missileStatus == 1) {
                    state.currPhase = HIT;
                    state.sigma = std::numeric_limits<TIME>::infinity();
                } else {
                    state.currPhase = FLYING;
                    state.sigma = TIME("00:00:01.000");
                }
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

        if (state.currPhase == FLYING) {
            get_messages<typename Aircraft_defs::outAircraftPosition>(bags).push_back(
                    messageAircraftPosition(state.aircraftPositionLat, state.aircraftPositionLong,
                                            state.aircraftAltitude));
        }

        return bags;
    }

    // time_advance function
    TIME time_advance() const {
        return state.sigma;
    }

    friend ostringstream &operator<<(ostringstream &os, const typename Aircraft<TIME>::state_type &currState) {

        switch (currState.currPhase) {

            case FLYING:
                os << "FLYING - Aircraft position: [" << currState.aircraftPositionLat << ","
                   << currState.aircraftPositionLong << "]";
                break;

            case MISSION_COMPLETED:
                os << "MISSION COMPLETED";
                break;

            case HIT:
                os << "HIT";
                break;
        }
        return os;
    }
};

#endif // _AIRCRAFT_HPP__