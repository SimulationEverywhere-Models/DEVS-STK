/**
* Gunner atomic model
*/

#ifndef __GUNNER_HPP__
#define __GUNNER_HPP__

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
struct Gunner_defs {
    struct inOrder : public in_port<messageOrder> {
    };
    struct outDistanceGunnerAircraft : public out_port<messageDistance> {
    };
    struct outGunnerStatus : public out_port<messageGunnerStatus> {
    };
};

// Atomic model class
template<typename TIME>
class Gunner {

    enum phase {
        READY, REPORTING, LAUNCHING, LOADING, UNAVAILABLE
    };

public:

    // default constructor
    Gunner() = default;

    Gunner(int ident) noexcept {
        state.currPhase = READY;
        state.sigma = TIME("00:00:05");
        state.gunnerID = ident;
        state.gunnerLatitude = SELECTED_GUNNERS_POSITION_LAT[ident - 1];
        state.gunnerLongitude = SELECTED_GUNNERS_POSITION_LONG[ident - 1];
        state.missileNumber = SELECTED_MISSILE_NUMBER[ident - 1];
        state.gunnerRange = 6000;
        state.missileReloadTime = TIME("00:00:40");
    }

    // state definition
    struct state_type {
        TIME sigma;
        phase currPhase;
        long double gunnerLatitude;
        long double gunnerLongitude;
        int missileNumber;
        double gunnerRange;
        int gunnerID;
        TIME missileReloadTime;
        double distanceGunnerAircraft;
    };

    state_type state;

    // ports definition
    using input_ports = tuple<typename Gunner_defs::inOrder>;
    using output_ports = tuple<typename Gunner_defs::outDistanceGunnerAircraft, typename Gunner_defs::outGunnerStatus>;

    // internal transition
    void internal_transition() {

        switch (state.currPhase) {
            case LAUNCHING:
                if (state.missileNumber > 0) {
                    state.currPhase = LOADING;
                    state.sigma = state.missileReloadTime;
                    break;
                } else {
                    state.currPhase = UNAVAILABLE;
                    state.sigma = std::numeric_limits<TIME>::infinity();
                    break;
                }
            case LOADING:
                state.currPhase = READY;
                state.sigma = TIME("00:00:05");
                break;
            case READY:
                state.currPhase = REPORTING;
                state.sigma = TIME("00:00:00");
                break;
            case REPORTING:
                state.currPhase = READY;
                state.sigma = TIME("00:00:05");
                break;
        }
    }

    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        for (const auto &x : get_messages<typename Gunner_defs::inOrder>(mbs)) {
            if ((state.currPhase == READY || state.currPhase == REPORTING) && state.gunnerID == x.gunnerID) {
                state.currPhase = LAUNCHING;
                state.missileNumber -= 1;
                state.sigma = TIME("00:00:00");
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

        if (state.currPhase == LAUNCHING) {
            get_messages<typename Gunner_defs::outDistanceGunnerAircraft>(bags).push_back(
                    messageDistance(state.gunnerID, state.gunnerRange, state.gunnerLatitude, state.gunnerLongitude,
                                    state.distanceGunnerAircraft));
        }

        if (state.currPhase == REPORTING) {
            get_messages<typename Gunner_defs::outGunnerStatus>(bags).push_back(
                    messageGunnerStatus(state.gunnerID, state.gunnerRange, state.gunnerLatitude,
                                        state.gunnerLongitude));
        }
        return bags;
    }

    // time_advance function
    TIME time_advance() const {
        return state.sigma;
    }

    friend ostringstream &operator<<(ostringstream &os, const typename Gunner<TIME>::state_type &currState) {

        switch (currState.currPhase) {

            case REPORTING:
                os << "Gunner Id: " << currState.gunnerID << " | REPORTING";
                break;

            case READY:
                os << "Gunner Id: " << currState.gunnerID << " | READY";
                break;

            case LAUNCHING:
                os << "Gunner Id: " << currState.gunnerID << " | HAS LAUNCHED A MISSILE";
                break;

            case LOADING:
                os << "Gunner Id: " << currState.gunnerID << " | LOADING A NEW MISSILE. Number of missiles left is: "
                   << currState.missileNumber;
                break;

            case UNAVAILABLE:
                os << "Gunner Id: " << currState.gunnerID << " | UNAVAILABLE";
                break;
        }

        return os;
    }
};

#endif //__GUNNER_HPP__