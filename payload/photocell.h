#ifndef __PHOTOCELL_H__
#define __PHOTOCELL_H__

// uncomment if you want to simulate the photocell
//#define SimulatePhotocell

typedef struct Photocell
{
    int pin;
} Photocell;

void InitializePhotocell(Photocell *photocell, int pin);

int Luminosity(Photocell *photocell);

#endif // __PHOTOCELL_H__