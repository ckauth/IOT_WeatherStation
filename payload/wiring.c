/*
* Developing IoT Solutions with Azure IoT - Microsoft Sample Code - Copyright (c) 2017 - Licensed MIT
*/

#include "wiring.h"

int setupWiringPi()
{
#ifdef __arm__
	return wiringPiSetup();
#elif __linux__
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	epochMilli = (uint64_t)ts.tv_sec * (uint64_t)1000 + (uint64_t)(ts.tv_nsec / 1000000L);
	return 0;
#endif
}

int setupWiringPiI2C(const int devId)
{
#ifdef __arm__
	return wiringPiI2CSetup(devId);
#else
	return 0;
#endif
}

void setPinMode(int pinNumber, int mode)
{
#ifdef __arm__
	pinMode(pinNumber, mode);
#else
	return; //no-op if not on Pi
#endif
}

void writeToPin(int pinNumber, int value)
{
#ifdef __arm__
	digitalWrite(pinNumber, value);
#else
	return; //no-op if not on Pi
#endif
}

int readFromPin(int pinNumber)
{
#ifdef __arm__
	digitalRead(pinNumber);
#else
	return 0; //no-op if not on Pi
#endif
}

void wait(int duration)
{
	fflush(stdout);

#ifdef __arm__
	delay(duration);
#elif _WIN32
	Sleep(duration);
#else
	usleep(duration * 1000);
#endif
}

unsigned int milli(void)
{
#ifdef __arm__
	return millis();
#elif __linux__
	uint64_t now;

	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	now = (uint64_t)ts.tv_sec * (uint64_t)1000 + (uint64_t)(ts.tv_nsec / 1000000L);

	return (uint32_t)(now - epochMilli);
#endif
}