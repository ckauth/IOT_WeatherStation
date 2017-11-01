#include "payload.h"
#include <wiring.h>

#define GREEN_LED_PIN 7
#define BLUE_LED_PIN 22
#define PHOTOCELL_PIN 1

void InitializePayload(Payload *payload)
{
    setupWiringPi();
    InitializeLed(&(payload->blueLed), BLUE_LED_PIN);
    InitializeLed(&(payload->greenLed), GREEN_LED_PIN);
    InitializePhotocell(&(payload->lightSensor), PHOTOCELL_PIN);
    InitializeBme280(&(payload->weatherSensor));
}

void BlinkGreenLed(Payload *payload)
{
    Blink(&(payload->greenLed));
}

void SwitchOffBlueLed(Payload *payload)
{
    SwitchOff(&(payload->blueLed));
}

void SwitchOnBlueLed(Payload *payload)
{
    SwitchOn(&(payload->blueLed));
}

PayloadData SenseEnvironment(Payload *payload)
{
    Bme280Data bme280Snapshot = Bme280Snapshot(&(payload->weatherSensor));

    PayloadData snapshot;
    snapshot.temperature = bme280Snapshot.temperature;          // C
    snapshot.pressure = bme280Snapshot.pressure;                // hPa
    snapshot.humidity = bme280Snapshot.humidity;                // %
    snapshot.luminosity = Luminosity(&(payload->lightSensor));  // a.u.

    return snapshot;
}
