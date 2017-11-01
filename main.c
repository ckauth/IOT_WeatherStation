#include "weatherstation.h"

#include <stdio.h>
#include <stdlib.h>

WeatherStation weatherStation;

// To control the program execution remotely
int DeviceMethodCallback(const char *method_name, const unsigned char *payload, size_t size, unsigned char **response, size_t *resp_size, void *userContextCallback)
{
    // Output debug info to console
    (void)userContextCallback;
    printf("\r\nDirect Method called\r\n");
    printf("Direct Method name:    %s\r\n", method_name);
    printf("Direct Method payload: %.*s\r\n", (int)size, (const char *)payload);

    // Process Method
    if (strcmp(method_name, "BroadcastWeather") == 0)
    {
        if (strcmp(payload, "[false]") == 0)
        {
            EnableBroadcast(&weatherStation, false);
            printf("Disable Weather Transmission\r\n");
        }
        else
        {
            EnableBroadcast(&weatherStation, true);
            printf("Enable Weather Transmission\r\n");
        }
    }

    // Create response
    int status = 200;
    char *RESPONSE_STRING = "{ \"Response\": \"Direct Method Received\" }";
    printf("\r\nResponse status: %d\r\n", status);
    printf("Response payload: %s\r\n\r\n", RESPONSE_STRING);
    *resp_size = strlen(RESPONSE_STRING);
    if ((*response = malloc(*resp_size)) == NULL)
    {
        status = -1;
    }
    else
    {
        (void)memcpy(*response, RESPONSE_STRING, *resp_size);
    }

    return status;
}


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("[Device] IoT Hub connection string should be passed as a parameter\r\n");
        return 1;
    }
    
    InitializeWeatherStation(&weatherStation, argv[1]);
    SenseWeatherPeriodically(&weatherStation);

    return 0;
}