/*
* Please refer to the BME280 library
https://github.com/adafruit/Adafruit_BME280_Library
and the datasheet
https://cdn-shop.adafruit.com/datasheets/BST-BME280_DS001-10.pdf
for register addresses, calibration and computation formulas
*/

#include "bme280.h"
#include "wiring.h"

#define BME280_ADDRESS                0x77

enum
{
    BME280_REGISTER_DIG_T1 = 0x88,
    BME280_REGISTER_DIG_T2 = 0x8A,
    BME280_REGISTER_DIG_T3 = 0x8C,
    BME280_REGISTER_DIG_P1 = 0x8E,
    BME280_REGISTER_DIG_P2 = 0x90,
    BME280_REGISTER_DIG_P3 = 0x92,
    BME280_REGISTER_DIG_P4 = 0x94,
    BME280_REGISTER_DIG_P5 = 0x96,
    BME280_REGISTER_DIG_P6 = 0x98,
    BME280_REGISTER_DIG_P7 = 0x9A,
    BME280_REGISTER_DIG_P8 = 0x9C,
    BME280_REGISTER_DIG_P9 = 0x9E,
    BME280_REGISTER_DIG_H1 = 0xA1,
    BME280_REGISTER_DIG_H2 = 0xE1,
    BME280_REGISTER_DIG_H3 = 0xE3,
    BME280_REGISTER_DIG_H4 = 0xE4,
    BME280_REGISTER_DIG_H5 = 0xE5,
    BME280_REGISTER_DIG_H6 = 0xE7,
    BME280_REGISTER_CHIPID = 0xD0,
    BME280_REGISTER_VERSION = 0xD1,
    BME280_REGISTER_SOFTRESET = 0xE0,
    BME280_RESET = 0xB6,
    BME280_REGISTER_CAL26 = 0xE1,
    BME280_REGISTER_CONTROLHUMID = 0xF2,
    BME280_REGISTER_CONTROL = 0xF4,
    BME280_REGISTER_CONFIG = 0xF5,
    BME280_REGISTER_PRESSUREDATA = 0xF7,
    BME280_REGISTER_TEMPDATA = 0xFA,
    BME280_REGISTER_HUMIDDATA = 0xFD,
} BME280_REGISTERS;

typedef struct RawData
{
    uint8_t pmsb;
    uint8_t plsb;
    uint8_t pxsb;

    uint8_t tmsb;
    uint8_t tlsb;
    uint8_t txsb;

    uint8_t hmsb;
    uint8_t hlsb;

    uint32_t temperature;
    uint32_t pressure;
    uint32_t humidity;  
} RawData;

static void ReadCalibrationData(Bme280 *bme280)
{
#ifdef __arm__
    bme280->calibrationData.dig_T1 = (uint16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_T1);
    bme280->calibrationData.dig_T2 = (int16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_T2);
    bme280->calibrationData.dig_T3 = (int16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_T3);

    bme280->calibrationData.dig_P1 = (uint16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_P1);
    bme280->calibrationData.dig_P2 = (int16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_P2);
    bme280->calibrationData.dig_P3 = (int16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_P3);
    bme280->calibrationData.dig_P4 = (int16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_P4);
    bme280->calibrationData.dig_P5 = (int16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_P5);
    bme280->calibrationData.dig_P6 = (int16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_P6);
    bme280->calibrationData.dig_P7 = (int16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_P7);
    bme280->calibrationData.dig_P8 = (int16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_P8);
    bme280->calibrationData.dig_P9 = (int16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_P9);

    bme280->calibrationData.dig_H1 = (uint8_t)wiringPiI2CReadReg8(bme280->fd, BME280_REGISTER_DIG_H1);
    bme280->calibrationData.dig_H2 = (int16_t)wiringPiI2CReadReg16(bme280->fd, BME280_REGISTER_DIG_H2);
    bme280->calibrationData.dig_H3 = (uint8_t)wiringPiI2CReadReg8(bme280->fd, BME280_REGISTER_DIG_H3);
    bme280->calibrationData.dig_H4 = (wiringPiI2CReadReg8(bme280->fd, BME280_REGISTER_DIG_H4) << 4) |
                                        (wiringPiI2CReadReg8(bme280->fd, BME280_REGISTER_DIG_H4+1) & 0xF);
    bme280->calibrationData.dig_H5 = (wiringPiI2CReadReg8(bme280->fd, BME280_REGISTER_DIG_H5+1) << 4) |
                                        (wiringPiI2CReadReg8(bme280->fd, BME280_REGISTER_DIG_H5) >> 4);
    bme280->calibrationData.dig_H6 = (int8_t)wiringPiI2CReadReg8(bme280->fd, BME280_REGISTER_DIG_H6);
#endif
}

static void MeasureRawData(Bme280 *bme280, RawData *rawData)
{
#if (defined(__arm__) && !defined(SimulateBme280))
    wiringPiI2CWrite(bme280->fd, 0xf7);
    
    rawData->pmsb = wiringPiI2CRead(bme280->fd);
    rawData->plsb = wiringPiI2CRead(bme280->fd);
    rawData->pxsb = wiringPiI2CRead(bme280->fd);

    rawData->tmsb = wiringPiI2CRead(bme280->fd);
    rawData->tlsb = wiringPiI2CRead(bme280->fd);
    rawData->txsb = wiringPiI2CRead(bme280->fd);

    rawData->hmsb = wiringPiI2CRead(bme280->fd);
    rawData->hlsb = wiringPiI2CRead(bme280->fd);

    rawData->temperature = 0;
    rawData->temperature = (rawData->temperature | rawData->tmsb) << 8;
    rawData->temperature = (rawData->temperature | rawData->tlsb) << 8;
    rawData->temperature = (rawData->temperature | rawData->txsb) >> 4;

    rawData->pressure = 0;
    rawData->pressure = (rawData->pressure | rawData->pmsb) << 8;
    rawData->pressure = (rawData->pressure | rawData->plsb) << 8;
    rawData->pressure = (rawData->pressure | rawData->pxsb) >> 4;
    
    rawData->humidity = 0;
    rawData->humidity = (rawData->humidity | rawData->hmsb) << 8;
    rawData->humidity = (rawData->humidity | rawData->hlsb);
#endif
}

static int32_t TemperatureCalibration(Bme280CalibrationData *cal, int32_t adc_T)
{
#if (defined(__arm__) && !defined(SimulateBme280))
    int32_t var1  = ((((adc_T>>3) - ((int32_t)cal->dig_T1 <<1))) * ((int32_t)cal->dig_T2)) >> 11;
    int32_t var2  = (((((adc_T>>4) - ((int32_t)cal->dig_T1)) * 
        ((adc_T>>4) - ((int32_t)cal->dig_T1))) >> 12) * ((int32_t)cal->dig_T3)) >> 14;
    return var1 + var2;
#else
    return 0;
#endif
}

static float RandomNumber(int min, int max) 
{
    return ((rand() % (int)(((max) + 1) - (min))) + (min));
}

static float CompensatedTemperature(int32_t t_fine)
{
#if (defined(__arm__) && !defined(SimulateBme280))
    float T  = (t_fine * 5 + 128) >> 8;
    return T/100.0;
#else
    return RandomNumber(0,100);
#endif
}

static float CompensatedPressure(int32_t adc_P, Bme280CalibrationData *cal, int32_t t_fine)
{
#if (defined(__arm__) && !defined(SimulateBme280))
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)cal->dig_P6;
    var2 = var2 + ((var1*(int64_t)cal->dig_P5)<<17);
    var2 = var2 + (((int64_t)cal->dig_P4)<<35);
    var1 = ((var1 * var1 * (int64_t)cal->dig_P3)>>8) + ((var1 * (int64_t)cal->dig_P2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)cal->dig_P1)>>33;

    if (var1 == 0)
    {
        return 0;  // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p<<31) - var2)*3125) / var1;
    var1 = (((int64_t)cal->dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)cal->dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)cal->dig_P7)<<4);
    return (float)p/256.0;
#else
    return RandomNumber(800, 1013); 
#endif
}

static float CompensatedHumidity(int32_t adc_H, Bme280CalibrationData *cal, int32_t t_fine)
{
#if (defined(__arm__) && !defined(SimulateBme280))
    int32_t v_x1_u32r;

    v_x1_u32r = (t_fine - ((int32_t)76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t)cal->dig_H4) << 20) -
        (((int32_t)cal->dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
        (((((((v_x1_u32r * ((int32_t)cal->dig_H6)) >> 10) *
        (((v_x1_u32r * ((int32_t)cal->dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
        ((int32_t)2097152)) * ((int32_t)cal->dig_H2) + 8192) >> 14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
        ((int32_t)cal->dig_H1)) >> 4));

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
  return (float)(v_x1_u32r>>12)/1024.0;
#else
    return RandomNumber(0,100);
#endif
}

void InitializeBme280(Bme280 *bme280)
{
  #if (defined(__arm__) && !defined(SimulateBme280))
    bme280->address = BME280_ADDRESS;
    bme280->fd = setupWiringPiI2C(bme280->address);
    if (bme280->fd < 0)
    {
        printf("Bme280 sensor not found");
        return;
    }

    ReadCalibrationData(bme280);  
    // humidity oversampling x 1
    wiringPiI2CWriteReg8(bme280->fd, 0xf2, 0x01);
    // pressure and temperature oversampling x 1, mode normal
    wiringPiI2CWriteReg8(bme280->fd, 0xf4, 0x27);
    // inactive time 1000ms, FIIR filter coefficient 4
    wiringPiI2CWriteReg8(bme280->fd, 0xf5, 0x54);
  #endif
}

Bme280Data Bme280Snapshot(Bme280 *bme280)
{
    RawData raw;
    MeasureRawData(bme280, &raw);

    Bme280Data data;
    int32_t t_fine = TemperatureCalibration(&(bme280->calibrationData), raw.temperature);
    data.temperature = CompensatedTemperature(t_fine);                                            // C
    data.pressure = CompensatedPressure(raw.pressure, &(bme280->calibrationData), t_fine) / 100;  // hPa
    data.humidity = CompensatedHumidity(raw.humidity, &(bme280->calibrationData), t_fine);        // %
    return data;
}