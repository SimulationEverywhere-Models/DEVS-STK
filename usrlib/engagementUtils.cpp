#include <iostream>
#include <fstream>
#include <string>
#include <boost/rational.hpp>
#include <cmath>

#define TORAD(X) (X)*(3.1415926535/180)
#define TODEG(X) (X)*(180/3.1415926535)
#define RADIUS_EARTH 6373000

using namespace std;

long double SELECTED_AIRCRAFT_INITIAL_POSITION_LAT = -31.30905;
long double SELECTED_AIRCRAFT_INITIAL_POSITION_LONG = 136.44974;
long double SELECTED_AIRCRAFT_FINAL_POSITION_LAT = -30.85932;
long double SELECTED_AIRCRAFT_FINAL_POSITION_LONG = 136.44974;
double SELECTED_AIRCRAFT_SPEED = 60.0;
double SELECTED_AIRCRAFT_ALTITUDE = 300.0;
long double SELECTED_RADAR_POSITION_LAT = -31.07870;
long double SELECTED_RADAR_POSITION_LONG = 136.47326;
double SELECTED_RADAR_RANGE = 20000.0;
long double SELECTED_C2_POSITION_LAT = -31.05130;
long double SELECTED_C2_POSITION_LONG = 136.46076;
long double SELECTED_GUNNERS_POSITION_LAT[3] = {-31.05007, -31.03852, -31.03363};
long double SELECTED_GUNNERS_POSITION_LONG[3] = {136.42800, 136.44373, 136.46193};
long double SELECTED_MISSILES_POSITION_LAT[3] = {-31.0504, -31.03830, -31.03361};
long double SELECTED_MISSILES_POSITION_LONG[3] = {136.428, 136.44394, 136.46159};
int SELECTED_MISSILE_NUMBER[3] = {4, 4, 4};
int SELECTED_MISSILE_POH_4_6 = 0;
int SELECTED_MISSILE_POH_1_4 = 0;
int SELECTED_MISSILE_POH_0_1 = 0;

// Allows the user to override base options with an external file if required
void ReadOptions(string sOptFile) {

    string sLine;
    string sKey;
    string sValue;

    cout << "Reading option file: " << sOptFile << "\n";

    ifstream fOptFile(sOptFile);
    if (fOptFile.is_open()) {

        while (getline(fOptFile, sLine)) {
            sKey = sLine.substr(0, sLine.find("="));
            sValue = sLine.substr(sLine.find("=") + 1, sLine.length());
            sValue.erase(std::remove(sValue.begin(), sValue.end(), '\n'), sValue.end());
            sValue.erase(std::remove(sValue.begin(), sValue.end(), '\r'), sValue.end());

            cout << setprecision(10);

            if (sKey.compare("AIRCRAFT_INITIAL_POSITION_LAT") == 0) {
                SELECTED_AIRCRAFT_INITIAL_POSITION_LAT = stold(sValue);
                cout<< "Updated 'aircraft initial latitude' to: " << SELECTED_AIRCRAFT_INITIAL_POSITION_LAT << "\n";

            } else if (sKey.compare("AIRCRAFT_INITIAL_POSITION_LONG") == 0) {
                SELECTED_AIRCRAFT_INITIAL_POSITION_LONG = stold(sValue);
                cout << "Updated 'aircraft initial longitude' to: " << SELECTED_AIRCRAFT_INITIAL_POSITION_LONG
                     << "\n";

            } else if (sKey.compare("AIRCRAFT_FINAL_POSITION_LAT") == 0) {
                SELECTED_AIRCRAFT_FINAL_POSITION_LAT = stold(sValue);
                cout << "Updated 'aircraft final latitude' to: " << SELECTED_AIRCRAFT_FINAL_POSITION_LAT << "\n";

            } else if (sKey.compare("AIRCRAFT_FINAL_POSITION_LONG") == 0) {
                SELECTED_AIRCRAFT_FINAL_POSITION_LONG = stold(sValue);
                cout << "Updated 'aircraft final longitude' to: " << SELECTED_AIRCRAFT_FINAL_POSITION_LONG << "\n";

            } else if (sKey.compare("AIRCRAFT_SPEED") == 0) {
                SELECTED_AIRCRAFT_SPEED = stod(sValue);
                cout << "Updated 'aircraft speed' to: " << SELECTED_AIRCRAFT_SPEED << " m/s\n";

            } else if (sKey.compare("AIRCRAFT_ALTITUDE") == 0) {
                SELECTED_AIRCRAFT_ALTITUDE = stod(sValue);
                cout << "Updated 'aircraft altitude' to: " << SELECTED_AIRCRAFT_ALTITUDE << " meters\n";

            } else if (sKey.compare("RADAR_POSITION_LAT") == 0) {
                SELECTED_RADAR_POSITION_LAT = stold(sValue);
                cout << "Updated 'radar latitude' to: " << SELECTED_RADAR_POSITION_LAT << "\n";

            } else if (sKey.compare("RADAR_POSITION_LONG") == 0) {
                SELECTED_RADAR_POSITION_LONG = stold(sValue);
                cout << "Updated 'radar longitude' to: " << SELECTED_RADAR_POSITION_LONG << "\n";

            } else if (sKey.compare("RADAR_RANGE") == 0) {
                SELECTED_RADAR_RANGE = stod(sValue);
                cout << "Updated 'radar range' to: " << SELECTED_RADAR_RANGE << " meters\n";

            } else if (sKey.compare("C2_POSITION_LAT") == 0) {
                SELECTED_C2_POSITION_LAT = stold(sValue);
                cout << "Updated 'C2 latitude' to: " << SELECTED_C2_POSITION_LAT << "\n";

            } else if (sKey.compare("C2_POSITION_LONG") == 0) {
                SELECTED_C2_POSITION_LONG = stold(sValue);
                cout << "Updated 'C2 longitude' to: " << SELECTED_C2_POSITION_LONG << "\n";

            } else if (sKey.compare("GUNNER_POSITION_LAT_ID1") == 0) {
                SELECTED_GUNNERS_POSITION_LAT[0] = stold(sValue);
                cout << "Updated 'Gunner ID1 latitude' to: " << SELECTED_GUNNERS_POSITION_LAT[0] << "\n";

            } else if (sKey.compare("GUNNER_POSITION_LONG_ID1") == 0) {
                SELECTED_GUNNERS_POSITION_LONG[0] = stold(sValue);
                cout << "Updated 'Gunner ID1 longitude' to: " << SELECTED_GUNNERS_POSITION_LONG[0] << "\n";

            } else if (sKey.compare("GUNNER_POSITION_LAT_ID2") == 0) {
                SELECTED_GUNNERS_POSITION_LAT[1] = stold(sValue);
                cout << "Updated 'Gunner ID2 latitude' to: " << SELECTED_GUNNERS_POSITION_LAT[1] << "\n";

            } else if (sKey.compare("GUNNER_POSITION_LONG_ID2") == 0) {
                SELECTED_GUNNERS_POSITION_LONG[1] = stold(sValue);
                cout << "Updated 'Gunner ID2 longitude' to: " << SELECTED_GUNNERS_POSITION_LONG[1] << "\n";

            } else if (sKey.compare("GUNNER_POSITION_LAT_ID3") == 0) {
                SELECTED_GUNNERS_POSITION_LAT[2] = stold(sValue);
                cout << "Updated 'Gunner ID3 latitude' to: " << SELECTED_GUNNERS_POSITION_LAT[2] << "\n";

            } else if (sKey.compare("GUNNER_POSITION_LONG_ID3") == 0) {
                SELECTED_GUNNERS_POSITION_LONG[2] = stold(sValue);
                cout << "Updated 'Gunner ID3 longitude' to: " << SELECTED_GUNNERS_POSITION_LONG[2] << "\n";

            } else if (sKey.compare("MISSILE_POSITION_LAT_ID1") == 0) {
                SELECTED_MISSILES_POSITION_LAT[0] = stold(sValue);
                cout << "Updated 'Missile ID1 latitude' to: " << SELECTED_MISSILES_POSITION_LAT[0] << "\n";

            } else if (sKey.compare("MISSILE_POSITION_LONG_ID1") == 0) {
                SELECTED_MISSILES_POSITION_LONG[0] = stold(sValue);
                cout << "Updated 'Missile ID1 longitude' to: " << SELECTED_MISSILES_POSITION_LONG[0] << "\n";

            } else if (sKey.compare("MISSILE_POSITION_LAT_ID2") == 0) {
                SELECTED_MISSILES_POSITION_LAT[1] = stold(sValue);
                cout << "Updated 'Missile ID2 latitude' to: " << SELECTED_MISSILES_POSITION_LAT[1] << "\n";

            } else if (sKey.compare("MISSILE_POSITION_LONG_ID2") == 0) {
                SELECTED_MISSILES_POSITION_LONG[1] = stold(sValue);
                cout << "Updated 'Missile ID2 longitude' to: " << SELECTED_MISSILES_POSITION_LONG[1] << "\n";

            } else if (sKey.compare("MISSILE_POSITION_LAT_ID3") == 0) {
                SELECTED_MISSILES_POSITION_LAT[2] = stold(sValue);
                cout << "Updated 'Missile ID3 latitude' to: " << SELECTED_MISSILES_POSITION_LAT[2] << "\n";

            } else if (sKey.compare("MISSILE_POSITION_LONG_ID3") == 0) {
                SELECTED_MISSILES_POSITION_LONG[2] = stold(sValue);
                cout << "Updated 'Missile ID3 longitude' to: " << SELECTED_MISSILES_POSITION_LONG[2] << "\n";

            } else if (sKey.compare("MISSILE_NUMBER_ID1") == 0) {
                SELECTED_MISSILE_NUMBER[0] = stoi(sValue);
                cout << "Updated 'missile number for gunner ID1' to: " << SELECTED_MISSILE_NUMBER[0] << "\n";

            } else if (sKey.compare("MISSILE_NUMBER_ID2") == 0) {
                SELECTED_MISSILE_NUMBER[1] = stoi(sValue);
                cout << "Updated 'missile number for gunner ID2' to: " << SELECTED_MISSILE_NUMBER[1] << "\n";

            } else if (sKey.compare("MISSILE_NUMBER_ID3") == 0) {
                SELECTED_MISSILE_NUMBER[2] = stoi(sValue);
                cout << "Updated 'missile number for gunner ID3' to: " << SELECTED_MISSILE_NUMBER[2] << "\n";

            } else if (sKey.compare("MISSILE_POH_4_6") == 0) {
                SELECTED_MISSILE_POH_4_6 = stoi(sValue);
                cout << "Updated 'probability of hit for missile launch range between 4 and 6 km' to: "
                     << SELECTED_MISSILE_POH_4_6 << "\n";

            } else if (sKey.compare("MISSILE_POH_1_4") == 0) {
                SELECTED_MISSILE_POH_1_4 = stoi(sValue);
                cout << "Updated 'probability of hit for missile launch range between 1 and 4 km' to: "
                     << SELECTED_MISSILE_POH_1_4 << "\n";

            } else if (sKey.compare("MISSILE_POH_0_1") == 0) {
                SELECTED_MISSILE_POH_0_1 = stoi(sValue);
                cout << "Updated 'probability of hit for missile launch range less than 1 km' to: "
                     << SELECTED_MISSILE_POH_0_1 << "\n";
            }
        }

        fOptFile.close();

    } else {
        cout << "Unable to open option file: " << sOptFile << "\n";
    }
}

double CalculateDistance(long double lat1, long double lon1, long double lat2, long double lon2) {
    double distance = RADIUS_EARTH * (2 * atan2(sqrt(
            (sin(((TORAD(lat2)) - (TORAD(lat1))) / 2) * sin(((TORAD(lat2)) - (TORAD(lat1))) / 2) +
             cos((TORAD(lat1))) * cos((TORAD(lat2))) * sin(((TORAD(lon2)) - (TORAD(lon1))) / 2) *
             sin(((TORAD(lon2)) - (TORAD(lon1))) / 2))), sqrt(1 - (sin(((TORAD(lat2)) - (TORAD(lat1))) / 2) *
                                                                   sin(((TORAD(lat2)) - (TORAD(lat1))) / 2) +
                                                                   cos((TORAD(lat1))) * cos((TORAD(lat2))) *
                                                                   sin(((TORAD(lon2)) - (TORAD(lon1))) / 2) *
                                                                   sin(((TORAD(lon2)) - (TORAD(lon1))) / 2)))));
    return distance;
}

long double CalculateBearing(long double lat1, long double lon1, long double lat2, long double lon2) {
    long double bearing = fmod(((atan2((sin((TORAD(lon2)) - (TORAD(lon1))) * cos((TORAD(lat2)))),
                                       (cos((TORAD(lat1))) * sin((TORAD(lat2))) -
                                        sin((TORAD(lat1))) * cos((TORAD(lat2))) *
                                        cos((TORAD(lon2)) - (TORAD(lon1)))))) * 180 / 3.1415926535 + 360), 360);
    return bearing;
}