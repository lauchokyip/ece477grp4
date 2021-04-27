#ifndef _THERMOPILE_H
#define _THERMOPILE_H

#include "stm32l4xx_hal.h"
#include "math.h"

#define TEMP_MAX 38
#define TEMP_MIN 35

uint32_t beta; //Kelvin
uint32_t R25;   //ohms
uint32_t T25;    //celsius
uint32_t Rsen;         //resistance calculated from adc thermistor input
uint32_t R1;
uint32_t Tsen;         //temperature to be found; celsius
uint32_t thermistorV; //mV
uint32_t thermopileV_raw; //mV
uint32_t thermopileV; //mV * 1000
uint32_t s;      //sensitivity conversion factor (x 10)
uint32_t ee;   //emissivity of object (x 100)
//uint32_t dirac;   //correction for filter transmission
uint32_t Tobj;
uint32_t factor; //factor for temperature scaling
uint32_t dirac_scale;

uint16_t raw;
uint16_t raw2;

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

void thermopile_init();
int getTemp();
int getTsen();
int getV();
int getR();

#endif
