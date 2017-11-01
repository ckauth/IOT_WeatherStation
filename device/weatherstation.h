#ifndef __WEATHERSTATION_H__
#define __WEATHERSTATION_H__

#include "iothub.h"
#include "payload.h"

typedef struct IoTHub IoTHub;

typedef struct WeatherStation
{
    char id[256];
    IoTHub iothub;
    bool isOnAir;
    Payload payload; 
} WeatherStation;

void InitializeWeatherStation(WeatherStation *station, char *connectionstring);

void EnableBroadcast(WeatherStation *station, bool transmit);

void SenseWeatherPeriodically(WeatherStation *station);

#endif // __WEATHERSTATION_H__