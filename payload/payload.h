#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

#include "bme280.h"
#include "led.h"
#include "photocell.h"

typedef struct PayloadData
{
    float temperature;
    float pressure;
    float humidity;
    int luminosity;
} PayloadData;

typedef struct Payload 
{
    Bme280 weatherSensor;
    Photocell lightSensor;
    Led greenLed;
    Led blueLed;
} Payload;

void InitializePayload(Payload* payload);

void BlinkGreenLed(Payload *payload);

void SwitchOffBlueLed(Payload *payload);

void SwitchOnBlueLed(Payload *payload);

PayloadData SenseEnvironment(Payload *payload);

#endif // __PAYLOAD_H__