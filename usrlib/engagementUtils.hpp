#ifndef __ENGAGEMENTUTILS_HPP__
#define __ENGAGEMENTUTILS_HPP__

#include <iostream>
#include <fstream>
#include <string>
#include <boost/rational.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <cmath>

using namespace std;

extern long double SELECTED_AIRCRAFT_INITIAL_POSITION_LAT;      //Aircraft initial latitude in decimal degrees
extern long double SELECTED_AIRCRAFT_INITIAL_POSITION_LONG;     //Aircraft initial longitude in decimal degrees
extern long double SELECTED_AIRCRAFT_FINAL_POSITION_LAT;        //Aircraft final latitude in decimal degrees
extern long double SELECTED_AIRCRAFT_FINAL_POSITION_LONG;       //Aircraft final longitude in decimal degrees
extern double SELECTED_AIRCRAFT_SPEED;                          //Aircraft speed in meters per second
extern double SELECTED_AIRCRAFT_ALTITUDE;                       //Aircraft altitude in meters
extern long double SELECTED_RADAR_POSITION_LAT;                 //Radar latitude in decimal degrees
extern long double SELECTED_RADAR_POSITION_LONG;                //Radar longitude in decimal degrees
extern double SELECTED_RADAR_RANGE;                             //Detection range of the radar in meters
extern long double SELECTED_C2_POSITION_LAT;                    //C2 latitude in decimal degrees
extern long double SELECTED_C2_POSITION_LONG;                   //C2 longitude in decimal degrees
extern long double SELECTED_GUNNERS_POSITION_LAT[3];            //Gunners latitude in decimal degrees
extern long double SELECTED_GUNNERS_POSITION_LONG[3];           //Gunners longitude in decimal degrees
extern long double SELECTED_MISSILES_POSITION_LAT[3];           //Missiles latitude in decimal degrees
extern long double SELECTED_MISSILES_POSITION_LONG[3];          //Missiles longitude in decimal degrees
extern int SELECTED_MISSILE_NUMBER[3];                          //Number of missiles available for each gunner
extern int SELECTED_MISSILE_POH_4_6;                            //Probability of hit of the missile at launching range between 4 and 6 km
extern int SELECTED_MISSILE_POH_1_4;                            //Probability of hit of the missile at launching range between 1 and 4 km
extern int SELECTED_MISSILE_POH_0_1;                            //Probability of hit of the missile at launching range less than 1 km

void ReadOptions(string sOptFile);		// Allows the user to override the default scenario parameters with an external file
double CalculateDistance(long double lat1, long double lon1, long double lat2, long double lon2); // Calculate the distance between two coordinates.
long double CalculateBearing(long double lat1, long double lon1, long double lat2, long double lon2); // Calculate bearing between two coordinates.

#endif  // __ENGAGEMENTUTILS_HPP__