#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_i2c.h"
#include "stm32l476g_discovery_glass_lcd.h"
#include "mytype.h"
#include "motion.h"

// Initialize everything
static float m_average_1 = 0;
static float m_average_2 = 0;
static float m_average_3 = 0;
static float m_average_4 = 0;
static float m_average_diff13 = 0;
static float m_average_diff24 = 0;

static float m_average_weight_1 = 0.05; // 0.3 very steep, 0.1 less step, 0.05 less steep
static float m_average_weight_2 = 0.05;
static float m_average_weight_3 = 0.05;
static float m_average_weight_4 = 0.05;
static float m_average_weight_diff13 = 0.05;
static float m_average_weight_diff24 = 0.05;

static float m_last_mark_value_1 = 0;
static float m_last_mark_value_2 = 0;
static float m_last_mark_value_3 = 0;
static float m_last_mark_value_4 = 0;
static float m_last_mark_value_diff13 = 0;
static float m_last_mark_value_diff24 = 0;

static uint32_t last_time = 0;
static uint32_t detect_interval = 100; // 50 ms

float threshold_presence = 10;
float threshold_movement = 5;

static uint8_t movement = MOVEMENT_NONE;


// Functions start here
uint8_t read_register(I2C_HandleTypeDef* hi2c, uint16_t register_value, uint8_t* data, int len) {
	 HAL_StatusTypeDef status;

	 status = HAL_I2C_Mem_Read(hi2c, AK975X_DEFAULT_ADDRESS , register_value, I2C_MEMADD_SIZE_8BIT, data , len, 100);
	    if(status != HAL_OK)
	   		return MOTION_ERROR;

	return MOTION_OK;
}

uint8_t write_register(I2C_HandleTypeDef* hi2c, uint16_t register_value, uint8_t* data, int len){
	HAL_StatusTypeDef status;

    status = HAL_I2C_Mem_Write(hi2c, AK975X_DEFAULT_ADDRESS , register_value, I2C_MEMADD_SIZE_8BIT, data, len, 100);
    if(status != HAL_OK)
   		return MOTION_ERROR;

    return MOTION_OK;
}

void refresh(I2C_HandleTypeDef* hi2c) {
	uint8_t data = 0; // dummy val
	read_register(hi2c, AK975X_ST2, &data, 1);
}


bool initialize_motion_sensor(I2C_HandleTypeDef* hi2c)
{
	uint8_t data = 0;
	uint8_t ret;
	// wait 3ms
	HAL_Delay(3);

	// soft Reset
    data = 0xFF;
    ret = write_register(hi2c, AK975X_CNTL2, &data, 1);
    if(ret == MOTION_ERROR)
    {
	  BSP_LCD_GLASS_DisplayString("Error0");
	  return false;
    }

    // check sensor type
	uint8_t sensor_type = 0;
	ret = read_register(hi2c,AK975X_WIA2, &sensor_type , 1);
	if(ret == MOTION_ERROR)
	{
		BSP_LCD_GLASS_DisplayString("Error1");
		return false;
	}
	if(sensor_type != 0x13)
	{
		BSP_LCD_GLASS_DisplayString("Error2");
		return false;
	}

	// set to continous read and output frequency to fastest
	data = (AK975X_FREQ_8_8HZ << 3) | AK975X_MODE_0;
	ret = write_register(hi2c, AK975X_ECNTL1, &data, 1);
    if(ret == MOTION_ERROR)
	{
	  BSP_LCD_GLASS_DisplayString("Error3");
	  return false;
	}

    // enable interrupt
    data = 0x1f;
    ret = write_register(hi2c, AK975X_EINTEN, &data, 1);
    if(ret == MOTION_ERROR)
    {
    	BSP_LCD_GLASS_DisplayString("Error4");
    	return false;
    }

    // reading dummy register return 255 so we don't need to check return value
    refresh(hi2c);

	return true;
}

bool is_motion_data_ready(I2C_HandleTypeDef* hi2c) {
	uint8_t data;
	uint8_t ret;
	ret = read_register(hi2c, AK975X_ST1, &data, 1);
	if(ret != MOTION_OK)
	{
		BSP_LCD_GLASS_DisplayString("Error6");
		return MOTION_ERROR;
	}
	return ((data & 0x01) == 0x01);
}

uint16_t get_raw_IR(I2C_HandleTypeDef* hi2c, uint16_t IR_address ) {
	uint8_t data[2];
	uint16_t ret;
	ret = read_register(hi2c, IR_address, data, 2);
	if(ret != MOTION_OK)
	{
		BSP_LCD_GLASS_DisplayString("Error7");
		return MOTION_ERROR;
	}
	ret = data[1] << 8 | data[0];
	return ret;
}

float get_IR_or_TMP(I2C_HandleTypeDef* hi2c, uint8_t which_IR) {
	uint16_t IR_address;
	switch (which_IR) {
	case 1:
		IR_address = AK975X_IR1;
		break;
	case 2:
		IR_address = AK975X_IR2;
		break;
	case 3:
		IR_address = AK975X_IR3;
		break;
	case 4:
		IR_address = AK975X_IR4;
		break;
	case 5:
		IR_address = AK975X_TMP;
		break;
	default:
		return 0;
	}
	short IR_val = (short) get_raw_IR(hi2c, IR_address);

	if(which_IR == 5){
		 float temperature = 26.75 + IR_val/ 512.0;
		 return temperature;
	}

	return 14286.8 * IR_val / 32768.0;
}

float add_data_point(float data, float m_average_weight, float m_average) {
	return m_average_weight * data + (1 - m_average_weight) * m_average;
}

float get_deriviative(uint8_t which_der) {
	float d = 0;
	switch(which_der) {
	case 1:
		d = m_average_1 - m_last_mark_value_1;
		m_last_mark_value_1 = m_average_1;
		return d;
	case 2:
		d = m_average_2 - m_last_mark_value_2;
		m_last_mark_value_2 = m_average_2;
		return d;
	case 3:
		d = m_average_3 - m_last_mark_value_3;
		m_last_mark_value_3 = m_average_3;
		return d;
	case 4:
		d = m_average_4 - m_last_mark_value_4;
		m_last_mark_value_4 = m_average_4;
		return d;
	case 5:
		d = m_average_diff13 - m_last_mark_value_diff13;
		m_last_mark_value_diff13 = m_average_diff13;
		return d;
	case 6:
		d = m_average_diff24 - m_last_mark_value_diff24;
		m_last_mark_value_diff24 = m_average_diff24;
		return d;
	default:
		return d;
	}
}

bool loop_motion_sensor(I2C_HandleTypeDef* hi2c) {

	uint32_t now = HAL_GetTick();

	if(is_motion_data_ready(hi2c) == false)
		return false;

	float IR1 = get_IR_or_TMP(hi2c, 1);
	float IR2 = get_IR_or_TMP(hi2c, 2);
	float IR3 = get_IR_or_TMP(hi2c, 3);
	float IR4 = get_IR_or_TMP(hi2c, 4);
	float diff13 = IR1 - IR3;
	float diff24 = IR2 - IR4;

	refresh(hi2c);

	m_average_1 = add_data_point(IR1, m_average_weight_1, m_average_1);
	m_average_2 = add_data_point(IR2, m_average_weight_2, m_average_2);
	m_average_3 = add_data_point(IR3, m_average_weight_3, m_average_3);
	m_average_4 = add_data_point(IR4, m_average_weight_4, m_average_4);
	m_average_diff13 = add_data_point(diff13, m_average_weight_diff13, m_average_diff13);
	m_average_diff24 = add_data_point(diff24, m_average_weight_diff24, m_average_diff24);

	if(now - last_time > detect_interval){

		float deriviative13 = get_deriviative(5);
		if(deriviative13 > threshold_movement)
		{
			 movement &= 0b11111100; 
             movement |= MOVEMENT_FROM_3_TO_1;
		}
		else if (deriviative13 < (-threshold_movement))
		{
			 movement &= 0b11111100; 
             movement |= MOVEMENT_FROM_1_TO_3;
		}

		float deriviative24 = get_deriviative(6);
		if(deriviative24 > threshold_movement)
		{
			 movement &= 0b11110011;
             movement |= MOVEMENT_FROM_4_TO_2;
		}
		else if (deriviative24 < (-threshold_movement))
		{ 
			movement &= 0b11110011;
            movement |= MOVEMENT_FROM_2_TO_4;
		}

		last_time = HAL_GetTick();
	}

	return true;
}

uint8_t motion_sensor_get_moment(){
	uint8_t ret = movement;
	movement = MOVEMENT_NONE;
	return ret;
}




