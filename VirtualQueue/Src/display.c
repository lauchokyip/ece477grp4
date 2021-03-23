#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_spi.h"
#include "display.h"
#include "mytype.h"

//static const int display_width = 800;
//static const int display_height = 400;
//static const int display_rotation = 0;

#define SPI_CS_GPIO GPIOE
#define SPI_CS_PIN GPIO_PIN_12
#define RST_PIN GPIO_PIN_10

uint8_t SPI_write_command(SPI_HandleTypeDef* hspi1, uint8_t* Txdata) {
	HAL_StatusTypeDef status;

	// toggle CS to low
	HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_RESET);

	// write WRITE command and write the data
	uint8_t reg_to_TX = RA8875_CMDWRITE;
	status  = HAL_SPI_Transmit(hspi1, &reg_to_TX, 1, 10);
	if(status != HAL_OK)
		return DISPLAY_ERROR;

	status = HAL_SPI_Transmit(hspi1, Txdata, 1, 10);
	if(status != HAL_OK)
		return DISPLAY_ERROR;

	// finish transfer, toggle CS to high
	HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_SET);

	return DISPLAY_OK;
}

uint8_t SPI_read_data(SPI_HandleTypeDef* hspi1, uint8_t* Rxdata) {
	HAL_StatusTypeDef status;

	// toggle CS to low
	HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_RESET);

	// write READ command to slve and read the data
	uint8_t reg_to_TX = RA8875_DATAREAD;
	status  = HAL_SPI_Transmit(hspi1, &reg_to_TX, 1, 10);
	if(status != HAL_OK)
		return DISPLAY_ERROR;

	status = HAL_SPI_Receive(hspi1, Rxdata, 1, 10);
	if(status != HAL_OK)
			return DISPLAY_ERROR;

	// finish transfer, toggle CS to high
	HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_SET);

	return DISPLAY_OK;
}

uint8_t SPI_read_register(SPI_HandleTypeDef* hspi1, uint8_t* Txdata, uint8_t* Rxdata) {
	uint8_t ret;
	ret = SPI_write_command(hspi1, Txdata);
	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	ret = SPI_read_data(hspi1, Rxdata);
	if (ret != DISPLAY_OK)
		return DISPLAY_ERROR;

	return DISPLAY_OK;
}

bool initialize_display(SPI_HandleTypeDef *hspi1) {
  uint8_t ret;
  uint8_t Txdata = 0;
  uint8_t Rxdata = 0;

  // CS pin to High before we start writing
  HAL_GPIO_WritePin(SPI_CS_GPIO, SPI_CS_PIN, GPIO_PIN_SET);

  // CS pin to LOW and to HIGH to reset
  HAL_GPIO_WritePin(GPIOE, RST_PIN, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(GPIOE, RST_PIN, GPIO_PIN_SET);
  HAL_Delay(100);

  // read Register to check the model
  Txdata = 0;
  ret = SPI_read_register(hspi1, &Txdata, &Rxdata);
  if(ret != DISPLAY_OK || Rxdata != 0x75)
	  return false;

  return true;
}
