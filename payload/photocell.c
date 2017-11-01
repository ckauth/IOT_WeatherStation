#include "photocell.h"
#include "wiring.h"

void InitializePhotocell(Photocell *photocell, int pin)
{
    photocell->pin = pin;
}

int Luminosity(Photocell *photocell)
{
#if (defined(__arm__) && !defined(SimulatePhotocell))
    // discharge capacitor
    setPinMode(photocell->pin, OUTPUT);
    writeToPin(photocell->pin, LOW);
    wait(100);

    // measure time until capacitor charge reaches threshold
    int chargeTime = 0;
    setPinMode(photocell->pin, INPUT);
    while (readFromPin(photocell->pin) == LOW)
    {
        chargeTime += 1;
    }
    return chargeTime;
#else
    return ((rand() % (int)(((1000) + 1) - (100))) + (100));
#endif
}