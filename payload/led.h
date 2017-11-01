#ifndef __LED_H__
#define __LED_H__

#include <stdbool.h>

typedef struct Led
{
    int pin;
} Led;

void InitializeLed(Led *led, int pin);

void Blink(Led *led);

void SwitchOff(Led *led);

void SwitchOn(Led *led);

#endif // __LED_H__