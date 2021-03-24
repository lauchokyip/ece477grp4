#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_spi.h"
#include "display.h"
#include "mytype.h"

static const uint16_t display_width = 800;
static const uint16_t display_height = 400;
static const uint8_t display_rotation = 0;
static uint8_t display_voffset;

#define SPI_CS_GPIO GPIOE
#define SPI_CS_PIN GPIO_PIN_12
#define RST_PIN GPIO_PIN_10

/**
 * Select the register to read or write
 *
 */
uint8_t SPI_write_command(SPI_HandleTypeDef* hspi, uint8_t reg_to_sel) {
	HAL_StatusTypeDef status;

	// toggle CS to low
	HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_RESET);

	uint8_t reg = RA8875_CMDWRITE;
	status  = HAL_SPI_Transmit(hspi, &reg, 1, 10);
	if(status != HAL_OK)
		return DISPLAY_ERROR;

	status = HAL_SPI_Transmit(hspi, &reg_to_sel, 1, 10);
	if(status != HAL_OK)
		return DISPLAY_ERROR;

	// finish transfer, toggle CS to high
	HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_SET);

	return DISPLAY_OK;
}

/**
 * Read the data from the current register
 *
 */
uint8_t SPI_read_data(SPI_HandleTypeDef* hspi, uint8_t* Rxdata) {
	HAL_StatusTypeDef status;
	uint8_t reg = RA8875_DATAREAD;

	// toggle CS to low
	HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_RESET);

	// write READ command to slave and read the data
	status  = HAL_SPI_Transmit(hspi, &reg, 1, 10);
	if(status != HAL_OK)
		return DISPLAY_ERROR;

	status = HAL_SPI_Receive(hspi, Rxdata, 1, 10);
	if(status != HAL_OK)
		return DISPLAY_ERROR;

	// finish transfer, toggle CS to high
	HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_SET);

	return DISPLAY_OK;
}

/**
 * Write the data to the current register
 *
 */
uint8_t SPI_write_data(SPI_HandleTypeDef* hspi, uint8_t* TxData) {
	HAL_StatusTypeDef status;
	uint8_t reg = RA8875_DATAWRITE;

	// toggle CS to low
	HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_RESET);

	// write WRITE command to slave and write the data
	status  = HAL_SPI_Transmit(hspi, &reg, 1, 10);
	if(status != HAL_OK)
		return DISPLAY_ERROR;

	status = HAL_SPI_Transmit(hspi, TxData, 1, 10);
	if(status != HAL_OK)
		return DISPLAY_ERROR;

	// finish transfer, toggle CS to high
	HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_SET);

	return DISPLAY_OK;
}

/**
 *  Select the register to read from and transmit the data using TxData
 *  Rxdata will be the returned data
 *
 */
uint8_t SPI_read_register(SPI_HandleTypeDef* hspi, uint8_t reg_to_sel, uint8_t* Rxdata) {
	uint8_t ret;
	// issue a read coomand and read data
	ret = SPI_write_command(hspi, reg_to_sel);
	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	ret = SPI_read_data(hspi, Rxdata);
	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	return DISPLAY_OK;
}

/**
 *  Select the register to write to and transmit the data using TxData
 *
 */
uint8_t SPI_write_register(SPI_HandleTypeDef* hspi, uint8_t reg_to_sel, uint8_t* TxData) {
	uint8_t ret;
	ret = SPI_write_command(hspi, reg_to_sel);
	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	ret = SPI_write_data(hspi, TxData);
	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	return DISPLAY_OK;
}

/**
 * Initialize the PLL
 *
 */
uint8_t initialize_PLL(SPI_HandleTypeDef* hspi){
	uint8_t ret = 0;
	uint8_t TxData = 0;

	TxData = RA8875_PLLC1_PLLDIV1 + 11;
	ret = SPI_write_register(hspi, RA8875_PLLC1, &TxData);
	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;
	HAL_Delay(1);

	TxData = RA8875_PLLC2_DIV4;
	ret = SPI_write_register(hspi,  RA8875_PLLC2, &TxData);
	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;
	HAL_Delay(1);

	return DISPLAY_OK;
}

/**
 *  Initialize the driver IC (clock setup, etc etc)
 */
uint8_t initialize_driverIC(SPI_HandleTypeDef* hspi){
	uint8_t ret = 0;
	uint8_t TxData = 0;

	// Timing value
	uint8_t   pixclk = RA8875_PCSR_PDATL | RA8875_PCSR_2CLK;
	uint8_t   hsync_nondisp = 26;
	uint8_t   hsync_start = 32;
	uint8_t   hsync_pw = 96;
	uint8_t   hsync_finetune = 0;
	uint16_t  vsync_nondisp = 32;
	uint16_t  vsync_start = 23;
	uint8_t   vsync_pw = 2;
	display_voffset = 0; // vertical offset can be changed over here

	initialize_PLL(hspi);

	TxData = RA8875_SYSR_16BPP | RA8875_SYSR_MCU8;
	ret = SPI_write_register(hspi, RA8875_SYSR , &TxData);
	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	TxData = pixclk;
	ret = SPI_write_register(hspi, RA8875_PCSR , &TxData);
	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;
	HAL_Delay(1);

	// Starting to set all the registers
	// return if errors occur in each segment (easier to debug)

	/* Horizontal settings registers */
	TxData = (display_width / 8) - 1;
	ret |= SPI_write_register(hspi, RA8875_HDWR , &TxData);
	TxData = RA8875_HNDFTR_DE_HIGH + hsync_finetune;
	ret |= SPI_write_register(hspi, RA8875_HNDFTR , &TxData);
	TxData =  (hsync_nondisp - hsync_finetune - 2) / 8;
	ret |= SPI_write_register(hspi, RA8875_HNDR , &TxData);
	TxData = hsync_start / 8 - 1;
	ret |= SPI_write_register(hspi, RA8875_HSTR , &TxData);
	TxData = RA8875_HPWR_LOW + (hsync_pw / 8 - 1);
	ret |= SPI_write_register(hspi, RA8875_HPWR , &TxData);

	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	/* Vertical settings registers */
	TxData = (display_height - 1 + display_voffset) & 0xFF;
	ret |= SPI_write_register(hspi, RA8875_VDHR0 , &TxData);
	TxData = (display_height - 1 + display_voffset) >> 8;
	ret |= SPI_write_register(hspi, RA8875_VDHR1 , &TxData);
	TxData =  vsync_nondisp - 1;
	ret |= SPI_write_register(hspi, RA8875_VNDR0 , &TxData);
	TxData =  vsync_nondisp >> 8;
	ret |= SPI_write_register(hspi, RA8875_VNDR1 , &TxData);
	TxData = vsync_start - 1;
	ret |= SPI_write_register(hspi, RA8875_VSTR0 , &TxData);
	TxData = vsync_start >> 8;
	ret |= SPI_write_register(hspi, RA8875_VSTR1 , &TxData);
	TxData = RA8875_VPWR_LOW + vsync_pw - 1;
	ret |= SPI_write_register(hspi, RA8875_VPWR , &TxData);

	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	/* Set active window X */
	TxData = 0;
	ret |= SPI_write_register(hspi, RA8875_HSAW0 , &TxData);
	TxData = 0;
	ret |= SPI_write_register(hspi, RA8875_HSAW1 , &TxData);
	TxData =  (display_width - 1) & 0xFF;
	ret |= SPI_write_register(hspi, RA8875_HEAW0 , &TxData);
	TxData = (display_width - 1) >> 8;
	ret |= SPI_write_register(hspi, RA8875_HEAW1 , &TxData);

	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	/* Set active window Y */
	TxData = 0 + display_voffset;
	ret |= SPI_write_register(hspi, RA8875_VSAW0 , &TxData);
	TxData = 0 + display_voffset;
	ret |= SPI_write_register(hspi, RA8875_VSAW1 , &TxData);
	TxData =  (display_height - 1 + display_voffset) & 0xFF;
	ret |= SPI_write_register(hspi, RA8875_VEAW0 , &TxData);
	TxData = (display_height - 1 + display_voffset) >> 8;
	ret |= SPI_write_register(hspi, RA8875_VEAW1 , &TxData);

	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	/* Clear the entire window */
	TxData = RA8875_MCLR_START | RA8875_MCLR_FULL;
	ret = SPI_write_register(hspi, RA8875_MCLR, &TxData);
	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	HAL_Delay(500);

	return DISPLAY_OK;
}

bool initialize_display(SPI_HandleTypeDef *hspi) {
  uint8_t ret;
  uint8_t reg;
  uint8_t RxData = 0;

  // CS pin to High before we start writing
  HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_SET);

  // CS pin to LOW and to HIGH to reset
  HAL_GPIO_WritePin(GPIOE, RST_PIN, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOE, RST_PIN, GPIO_PIN_SET);
  HAL_Delay(100);

  // read Register 0 to check the model
  reg = 0;
  ret = SPI_read_register(hspi, reg, &RxData);
  if(ret != DISPLAY_OK || RxData != 0x75)
	  return false;

  ret = initialize_driverIC(hspi);
  if(ret != DISPLAY_OK)
	  return false;

  return true;
}

void display_on(SPI_HandleTypeDef *hspi, bool on){
	uint8_t TxData = 0;

	if(on == true)
	{
		TxData = RA8875_PWRR_NORMAL | RA8875_PWRR_DISPON;
		SPI_write_register(hspi, RA8875_PWRR, &TxData);
	}
	else
	{
		TxData = RA8875_PWRR_NORMAL | RA8875_PWRR_DISPOFF;
		SPI_write_register(hspi, RA8875_PWRR, &TxData);
	}
	return;
}

void GPIOX_on(SPI_HandleTypeDef *hspi, bool on){
	uint8_t TxData = 0;

	if(on == true)
	{
		TxData = 1;
		SPI_write_register(hspi, RA8875_GPIOX, &TxData);
	}
	else
	{
		TxData = 0;
		SPI_write_register(hspi, RA8875_GPIOX, &TxData);
	}
	return;
}

void PWM1_config(SPI_HandleTypeDef *hspi, bool on, uint8_t clock){
	uint8_t TxData = 0;

	if(on == true)
	{
		TxData = RA8875_P1CR_ENABLE | (clock & 0xF);
		SPI_write_register(hspi, RA8875_P1CR, &TxData);
	}
	else
	{
		TxData = RA8875_P1CR_DISABLE | (clock & 0xF);
		SPI_write_register(hspi, RA8875_P1CR, &TxData);
	}
	return;
}

void PWM1_out(SPI_HandleTypeDef *hspi, uint8_t duty_cycle){
	uint8_t TxData = duty_cycle;
	SPI_write_register(hspi, RA8875_P1DCR, &TxData);
}



