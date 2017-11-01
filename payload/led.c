#include "led.h"

#include "./wiring.h"

void InitializeLed(Led *led, int pin)
{
    led->pin = pin;
    setPinMode(led->pin, OUTPUT);
    SwitchOff(led);
}

void Blink(Led *led)
{
    SwitchOn(led);
    wait(200);
    SwitchOff(led);
}

void SwitchOff(Led *led)
{
    writeToPin(led->pin, LOW);
}

void SwitchOn(Led *led)
{
    writeToPin(led->pin, HIGH);
}
