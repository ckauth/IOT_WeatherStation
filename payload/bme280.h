#ifndef __BME280_H__
#define __BME280_H__

#include <inttypes.h>

// uncomment if you want to simulate the bme280
//#define SimulateBme280

typedef struct Bme280CalibrationData
{
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;
} Bme280CalibrationData;

typedef struct Bme280Snapshot
{
    float temperature;
    float pressure;
    float humidity;
} Bme280Data;

typedef struct Bme280
{
    Bme280CalibrationData calibrationData;
    int address;
    int fd;
} Bme280;

void InitializeBme280(Bme280 *bme280);

Bme280Data Bme280Snapshot(Bme280 *bme280);

#endif // __BME280_H__