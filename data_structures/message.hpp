#ifndef __ENGAGEMENT_SIMULATION_MESSAGE_HPP__
#define __ENGAGEMENT_SIMULATION_MESSAGE_HPP__

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

// messageMissileStatus
struct messageMissileStatus {
    messageMissileStatus() {}

    messageMissileStatus(long double iMissileLatitude, long double iMissileLongitude, int iMissileStatus)
            : missileLatitude(iMissileLatitude), missileLongitude(iMissileLongitude), missileStatus(iMissileStatus) {}

    long double missileLatitude;
    long double missileLongitude;
    int missileStatus;
};

ostream &operator<<(ostream &os, const messageMissileStatus &msg);

istream &operator>>(istream &is, messageMissileStatus &msg);

// messageAircraftPosition
struct messageAircraftPosition {
    messageAircraftPosition() {}

    messageAircraftPosition(long double iplat, long double iplong, double iAircraftAltitude) : aircraftLatitude(iplat),
                                                                                               aircraftLongitude(
                                                                                                       iplong),
                                                                                               aircraftAltitude(
                                                                                                       iAircraftAltitude) {}

    double aircraftAltitude;
    long double aircraftLatitude;
    long double aircraftLongitude;
};

ostream &operator<<(ostream &os, const messageAircraftPosition &msg);

istream &operator>>(istream &is, messageAircraftPosition &msg);

// messageDetectionPosition
struct messageDetectedPosition {
    messageDetectedPosition() {}

    messageDetectedPosition(double iRadarRange, long double iRadarLatitude, long double iRadarLongitude,
                            long double iDetectedAircraftLatitude, long double iDetectedAircraftLongitude) : radarRange(
            iRadarRange), radarLatitude(iRadarLatitude), radarLongitude(iRadarLongitude), detectedAircraftLatitude(
            iDetectedAircraftLatitude), detectedAircraftLongitude(iDetectedAircraftLongitude) {}

    double radarRange;
    long double radarLatitude;
    long double radarLongitude;
    long double detectedAircraftLatitude;
    long double detectedAircraftLongitude;
};

ostream &operator<<(ostream &os, const messageDetectedPosition &msg);

istream &operator>>(istream &is, messageDetectedPosition &msg);

// messageDistance
struct messageDistance {
    messageDistance() {}

    messageDistance(int iGunnerID, double iGunnerRange, long double iGunnerLatitude, long double iGunnerLongitude,
                    double iDistance) : gunnerID(iGunnerID), gunnerRange(iGunnerRange), gunnerLatitude(iGunnerLatitude),
                                        gunnerLongitude(iGunnerLongitude), distance(iDistance) {}

    int gunnerID;
    double gunnerRange;
    long double gunnerLatitude;
    long double gunnerLongitude;
    double distance;
};

ostream &operator<<(ostream &os, const messageDistance &msg);

istream &operator>>(istream &is, messageDistance &msg);

// messageGunnerStatus
struct messageGunnerStatus {
    messageGunnerStatus() {}

    messageGunnerStatus(int iGunnerID, double iGunnerRange, long double iGunnerLatitude, long double iGunnerLongitude)
            : gunnerID(iGunnerID), gunnerRange(iGunnerRange), gunnerLatitude(iGunnerLatitude),
              gunnerLongitude(iGunnerLongitude) {}

    int gunnerID;
    double gunnerRange;
    long double gunnerLatitude;
    long double gunnerLongitude;
};

ostream &operator<<(ostream &os, const messageGunnerStatus &msg);

istream &operator>>(istream &is, messageGunnerStatus &msg);


// messageOrder
struct messageOrder {
    messageOrder() {}

    messageOrder(int iGunnerID, double iDistance, long double iC2Latitude, long double iC2Longitude) : gunnerID(
            iGunnerID), distance(iDistance), C2Latitude(iC2Latitude), C2Longitude(iC2Longitude) {}

    int gunnerID;
    double distance;
    long double C2Latitude;
    long double C2Longitude;
};

ostream &operator<<(ostream &os, const messageOrder &msg);

istream &operator>>(istream &is, messageOrder &msg);

#endif // __ENGAGEMENT_SIMULATION_MESSAGE_HPP__