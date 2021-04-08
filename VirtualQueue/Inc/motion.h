#ifndef _MOTION_H
#define _MOTION_H

#include "mytype.h"
#include "stm32l4xx_hal_i2c.h"

#define MOTION_OK 0
#define MOTION_ERROR 255

#define AK975X_DEFAULT_ADDRESS 0x64 << 1//7-bit unshifted default I2C Address

//Register addresses
#define AK975X_WIA1       0x00
#define AK975X_WIA2 0x01 //Device ID
#define AK975X_INFO1 0x02
#define SENSORVERSION_AK9750 0x00
#define SENSORVERSION_AK9753 0x01
#define AK975X_ST1 0x05
#define AK975X_IR1 0x06
#define AK975X_IR2 0x08
#define AK975X_IR3 0x0A
#define AK975X_IR4 0x0C
#define AK975X_TMP 0x0E
#define AK975X_ST2 0x10 //Dummy register
#define AK975X_ETH13H 0x11
#define AK975X_ETH13L 0x13
#define AK975X_ETH24H 0x15
#define AK975X_ETH24L 0x17
#define AK975X_EHYS13 0x19
#define AK975X_EHYS24 0x1A
#define AK975X_EINTEN 0x1B
#define AK975X_ECNTL1 0x1C
#define AK975X_CNTL2 0x1D

//Valid sensor modes - Register ECNTL1
#define AK975X_MODE_STANDBY 0b000
#define AK975X_MODE_EEPROM_ACCESS 0b001
#define AK975X_MODE_SINGLE_SHOT 0b010
#define AK975X_MODE_0 0b100
#define AK975X_MODE_1 0b101
#define AK975X_MODE_2 0b110
#define AK975X_MODE_3 0b111

//Valid digital filter cutoff frequencies
#define AK975X_FREQ_0_3HZ 0b000
#define AK975X_FREQ_0_6HZ 0b001
#define AK975X_FREQ_1_1HZ 0b010
#define AK975X_FREQ_2_2HZ 0b011
#define AK975X_FREQ_4_4HZ 0b100
#define AK975X_FREQ_8_8HZ 0b101


//Movement
#define MOVEMENT_NONE        0b0000
#define MOVEMENT_FROM_1_TO_3 0b0001
#define MOVEMENT_FROM_3_TO_1 0b0010
#define MOVEMENT_FROM_2_TO_4 0b0100
#define MOVEMENT_FROM_4_TO_2 0b1000


bool initialize_motion_sensor(I2C_HandleTypeDef* hi2c);
void refresh(I2C_HandleTypeDef* hi2c);
bool is_motion_data_ready(I2C_HandleTypeDef* hi2c);
uint16_t get_raw_IR(I2C_HandleTypeDef* hi2c, uint16_t IR_address );
float get_IR_or_TMP(I2C_HandleTypeDef* hi2c, uint8_t which_IR);
bool loop_motion_sensor(I2C_HandleTypeDef* hi2c);
uint8_t motion_sensor_get_moment();

#endif
