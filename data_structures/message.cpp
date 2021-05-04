#include <math.h>
#include <iostream>
#include <string>
#include <iomanip>

#include "message.hpp"

// messageMissileStatus
ostream &operator<<(ostream &os, const messageMissileStatus &msg) {
    os << "Missile position (lat, long): " << "<" << msg.missileLatitude << "," << msg.missileLongitude << ">"
       << " | Missile status is: " << msg.missileStatus;
    return os;
}

istream &operator>>(istream &is, messageMissileStatus &msg) {
    is >> msg.missileLatitude;
    is >> msg.missileLongitude;
    is >> msg.missileStatus;
    return is;
}

// messageAircraftPosition
ostream &operator<<(ostream &os, const messageAircraftPosition &msg) {
    os << setprecision(10) << "Aircraft position (lat, long, altitude): " << "<" << msg.aircraftLatitude << ","
       << msg.aircraftLongitude << "," << msg.aircraftAltitude << ">";
    return os;
}

istream &operator>>(istream &is, messageAircraftPosition &msg) {
    is >> msg.aircraftLatitude;
    is >> msg.aircraftLongitude;
    is >> msg.aircraftAltitude;
    return is;
}

// messageDetectedPosition
ostream &operator<<(ostream &os, const messageDetectedPosition &msg) {
    os << "Radar range: " << msg.radarRange << " m | Radar position (lat, long): " << "<" << msg.radarLatitude << ","
       << msg.radarLongitude << ">" << setprecision(10) << " | Detected aircraft position (lat, long): "
       << msg.detectedAircraftLatitude << " , " << msg.detectedAircraftLongitude;
    return os;
}

istream &operator>>(istream &is, messageDetectedPosition &msg) {
    is >> msg.radarRange;
    is >> msg.radarLatitude;
    is >> msg.radarLongitude;
    is >> msg.detectedAircraftLatitude;
    is >> msg.detectedAircraftLongitude;
    return is;
}

// messageDistance
ostream &operator<<(ostream &os, const messageDistance &msg) {
    //os << "<" << msg.distance << ">";
    os << "Gunner id: " << msg.gunnerID << " Gunner range: " << msg.gunnerRange << " m | Gunner position (lat, long): "
       << "<" << msg.gunnerLatitude << "," << msg.gunnerLongitude << ">" << " | Distance to aircraft: " << msg.distance
       << " meters";
    return os;
}

istream &operator>>(istream &is, messageDistance &msg) {
    is >> msg.gunnerID;
    is >> msg.gunnerRange;
    is >> msg.gunnerLatitude;
    is >> msg.gunnerLongitude;
    is >> msg.distance;
    return is;
}

// messageOrder
ostream &operator<<(ostream &os, const messageOrder &msg) {
    os << "Gunner id: " << msg.gunnerID << " | Distance to aircraft: " << msg.distance
       << " meters | C2 position (lat, long): " << "<" << msg.C2Latitude << "," << msg.C2Longitude << ">";
    return os;
}

istream &operator>>(istream &is, messageOrder &msg) {
    is >> msg.gunnerID;
    is >> msg.distance;
    is >> msg.C2Latitude;
    is >> msg.C2Longitude;
    return is;
}

// messageGunnerStatus
ostream &operator<<(ostream &os, const messageGunnerStatus &msg) {
    os << "Gunner id: " << msg.gunnerID << " | Gunner range: " << msg.gunnerRange
       << " m | Gunner position (lat, long): " << "<" << msg.gunnerLatitude << "," << msg.gunnerLongitude << ">";
    return os;
}

istream &operator>>(istream &is, messageGunnerStatus &msg) {
    is >> msg.gunnerID;
    is >> msg.gunnerRange;
    is >> msg.gunnerLatitude;
    is >> msg.gunnerLongitude;
    return is;
}