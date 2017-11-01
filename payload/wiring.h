/*
* Developing IoT Solutions with Azure IoT - Microsoft Sample Code - Copyright (c) 2017 - Licensed MIT
*/

#ifndef __WIRINGPI_H__
#define __WIRINGPI_H__

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#ifdef __arm__
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#elif _WIN32
#include <Windows.h>
#else
#include <linux/time.h>
#include <unistd.h>
#endif

#ifndef __arm__
#define HIGH 1
#define LOW 0
#define OUTPUT 0
#define INPUT 1
static uint64_t epochMilli;
#endif

extern int setupWiringPi();

extern int setupWiringPiI2C(const int devId);

extern void setPinMode(int pinNumber, int mode);

extern void writeToPin(int pinNumber, int value);

extern int readFromPin(int pinNumber);

extern void wait(int duration);

extern unsigned int milli(void);

#endif // __WIRINGPI_H__
