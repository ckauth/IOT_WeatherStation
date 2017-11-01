#include <stdint.h>
#include <string.h>
#include <time.h>

#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"

#include "iothub.h"
#include "payload.h"
#include "weatherstation.h"

static void GetTime(char *timeString, int stringLength)
{
    time_t rawtime;
    struct tm *timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    snprintf(timeString, stringLength, "%s", asctime(timeinfo));
    timeString[strlen(timeString) - 1] = '\0';
}

static void ComposeWeatherReport(WeatherStation *station, char *reportString, int reportLength)
{
    static int reportCount = 0;
    ++reportCount;

    // sense the environment
    PayloadData environment = SenseEnvironment(&(station->payload));
    
    // get the local time
    char localTime[reportLength];
    GetTime(localTime, reportLength);
    
    // compose the report 
    snprintf(reportString, reportLength,
        "{\"deviceId\": \"%s\", \"reportId\": %d, \"brightness\": %d, \"humidity\": %.2f, \"pressure\": %.2f, \"temperature\": %.2f, \"Timestamp\": \"%s\"}",
        station->id,
        reportCount,
        environment.luminosity,
        environment.humidity,
        environment.pressure,
        environment.temperature,
        localTime);
}

static void DisplayWeatherReport(WeatherStation* station, char report[])
{
    printf("Weather report: %s\r\n", report);
}

static void ExtractStationId(WeatherStation *station, char *connectionstring)
{
    char *substr = strstr(connectionstring, "DeviceId=");

    if (substr == NULL)
    {
        printf("[Device] ERROR: Cannot parse device id from IoT device connection string: %s\n", connectionstring);
    }

    substr += 9;     // skip "DeviceId="
    char *semicolon = strstr(substr, ";");
    int length = semicolon == NULL ? strlen(substr) : semicolon - substr;
    memcpy(station->id, substr, length);
    station->id[length] = '\0';
}

void InitializeWeatherStation(WeatherStation* station, char *connectionstring)
{
    ExtractStationId(station, connectionstring);
    InitializePayload(&(station->payload));
    InitializeIoTHub(&(station->iothub), connectionstring);
    EnableBroadcast(station, true);
}

void EnableBroadcast(WeatherStation *station, bool transmit)
{
    station->isOnAir = transmit;
    if (transmit)
    {
        SwitchOnBlueLed(&(station->payload));
    }
    else
    {
        SwitchOffBlueLed(&(station->payload));
    }
}

void SenseWeatherPeriodically(WeatherStation* station)
{
    do
    {
        // Visually indicate that the measurement is happening
        BlinkGreenLed(&(station->payload));

        // Sense the weather
        char report[256];
        ComposeWeatherReport(station, report, 256);
        DisplayWeatherReport(station, report);

        // Report the weather to the IoT-Hub
        if (IsIoTHubConnectionEstablished(&(station->iothub)) && station->isOnAir)
        {   
            SendMessageToIoTHub(&(station->iothub), report);
        }
            
        // Wait for approximately 60" before next measurement, but keep listening to IoT-Hub
        for (int t=0; t < 12; ++t)
        {
            DoWork(&(station->iothub));
            ThreadAPI_Sleep(5000);
        }
    } while (true);
}