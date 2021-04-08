#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_spi.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "display.h"
#include "mytype.h"

static const uint16_t display_width = 800;
static const uint16_t display_height = 480; // This is a hack to get rid of weird layer
static const uint8_t next_line_height = 35;

static uint8_t display_voffset;

#define SPI_CS_GPIO GPIOE
#define SPI_CS_PIN GPIO_PIN_12
#define RST_PIN GPIO_PIN_10

//////////////////////////////////////////////////////////////////////////////////
//
//  Start of helper function
//
//////////////////////////////////////////////////////////////////////////////////

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
 *  Select the register to write to and transmit the data using TxData
 *	return true if expected status has been reached
 *  return false if SPI_read has errors 
 */
bool wait_poll(SPI_HandleTypeDef* hspi,uint8_t reg_to_sel, uint8_t wait_flag) {
	uint8_t RxData;
	uint8_t ret;
	// wait for the command to finish
	while(1) {
		RxData = 0;
		ret = SPI_read_register(hspi, reg_to_sel, &RxData);
		if(ret != DISPLAY_OK)
			return false;
		if(!(RxData & wait_flag))
			return true;
	}
	// unreach but return to avoid compiler yelling
	return false;
}

/**
 *  Helper function for higher level rectangle drawing code
 *
 *  Y1 X----------------X1 Y1
 *  |                      |
 *	|					   |
 *	|					   |
 *	|					   |
 *	Y  X----------------X1 Y
 */
void rect_helper(SPI_HandleTypeDef* hspi, int16_t x, int16_t y, int16_t x1, int16_t y1, uint16_t color, bool filled){
	uint8_t TxData = 0;

	// Set X
	SPI_write_command(hspi, 0x91);
	TxData = x;
	SPI_write_data(hspi, &TxData);
	SPI_write_command(hspi, 0x92);
	TxData = x >> 8;
	SPI_write_data(hspi, &TxData);

	// Set Y
	SPI_write_command(hspi, 0x93);
	TxData = y;
	SPI_write_data(hspi, &TxData);
	SPI_write_command(hspi, 0x94);
	TxData = y >> 8;
	SPI_write_data(hspi, &TxData);

	// set X1
	SPI_write_command(hspi, 0x95);
	TxData = x1;
	SPI_write_data(hspi, &TxData);
	SPI_write_command(hspi, 0x96);
	TxData = x1 >> 8;
	SPI_write_data(hspi, &TxData);

	// set Y1
	SPI_write_command(hspi, 0x97);
	TxData = y1;
	SPI_write_data(hspi, &TxData);
	SPI_write_command(hspi, 0x98);
	TxData = y1 >> 8;
	SPI_write_data(hspi, &TxData);

	// set Color
	SPI_write_command(hspi, 0x63);
	TxData = (color & 0xf800) >> 11;
	SPI_write_data(hspi, &TxData);
	SPI_write_command(hspi, 0x64);
	TxData = (color & 0x07e0) >> 5;
	SPI_write_data(hspi, &TxData);
	SPI_write_command(hspi, 0x65);
	TxData = color & 0x001f;
	SPI_write_data(hspi, &TxData);

	// Draw
	SPI_write_command(hspi, RA8875_DCR);
	if(filled == true) {
		TxData = 0xB0;
		SPI_write_data(hspi, &TxData);
	} else {
		TxData = 0x90;
		SPI_write_data(hspi, &TxData);
	}

	wait_poll(hspi, RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
}


//////////////////////////////////////////////////////////////////////////////////
//
//  Start of initialization function
//
//////////////////////////////////////////////////////////////////////////////////


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

/**
 *  initialize the LCD driver and any HW required by the display.
 *  Returns true if display is successfully been initialized
 */
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

/**
 *  Power on the display
 */
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

/**
 *  Set the Extra General Purpose IO Register
 *
 */
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

/**
 *  Configure the PWM1 clock
 */
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

/**
 * Configure the duty cycle of the PWM
 *
 */
void PWM1_out(SPI_HandleTypeDef *hspi, uint8_t duty_cycle){
	uint8_t TxData = duty_cycle;
	SPI_write_register(hspi, RA8875_P1DCR, &TxData);
}

//////////////////////////////////////////////////////////////////////////////////
//
//  Start of  drawing function
//
//////////////////////////////////////////////////////////////////////////////////


/**
 *  Fill the screen with color
 *
 */
void fill_screen(SPI_HandleTypeDef *hspi, uint16_t color) {
	rect_helper(hspi, 0, 0, display_width - 1, display_height - 1, color, true);
}

/**
 *  Sets the display in text mode
 *
 */
void text_mode(SPI_HandleTypeDef *hspi) {
	uint8_t RxData = 0;
	// Set text mode
	SPI_read_register(hspi, RA8875_MWCR0, &RxData);
	RxData |= RA8875_MWCR0_TXTMODE;
	SPI_write_data(hspi, &RxData);

	// Select internal (ROM) font
	SPI_read_register(hspi, 0x21, &RxData);
	RxData &= ~((1 << 7) | (1 << 5)); // clear bits 7 and 5
	SPI_write_data(hspi, &RxData);
}

/**
 *
     Enable Cursor Visibility and Blink
     Here we set bits 6 and 5 in 40h
     As well as the set the blink rate in 44h
     The rate is 0 through max 255
     the lower the number the faster it blinks (00h is 1 frame time,
     FFh is 256 Frames time.
     Blink Time (sec) = BTCR[44h]x(1/Frame_rate)
 *
 */
void cursor_blink(SPI_HandleTypeDef *hspi, uint8_t rate) {
	uint8_t RxData = 0;
	uint8_t TxData = 0;

	// Set text mode
	SPI_read_register(hspi, RA8875_MWCR0, &RxData);
	RxData |= RA8875_MWCR0_CURSOR;
	SPI_write_data(hspi, &RxData);

	// Select internal (ROM) font
	SPI_read_register(hspi, RA8875_MWCR0, &RxData);
	RxData |= RA8875_MWCR0_BLINK;
	SPI_write_data(hspi, &RxData);

	TxData = rate;
	SPI_write_register(hspi, RA8875_BTCR, &TxData);
}

/**
 *  Set the location of the cursor
 *
 */
void set_cursor(SPI_HandleTypeDef *hspi, uint16_t x, uint16_t y){
	uint8_t TxData = 0;

	TxData = x & 0xFF;
	SPI_write_register(hspi, 0x2A, &TxData);

	TxData = x >> 8;
	SPI_write_register(hspi, 0x2B, &TxData);

	TxData = y & 0xFF;
	SPI_write_register(hspi, 0x2C, &TxData);

	TxData = y >> 8;
	SPI_write_register(hspi, 0x2D, &TxData);
}

/**
 *  Write the string to the screen
 */
void text_write(SPI_HandleTypeDef *hspi, char* buffer, uint16_t len) {
	SPI_write_command(hspi, RA8875_MRWC);

	for(uint16_t i = 0; i < len; i++) {
		uint8_t each_char = buffer[i];
		SPI_write_data(hspi, &each_char);
	}
	HAL_Delay(1);
}

void clear_screen(SPI_HandleTypeDef *hspi) {
	uint8_t TxData = 0;

	TxData = RA8875_MCLR_START | RA8875_MCLR_FULL;
	SPI_write_register(hspi, RA8875_MCLR, &TxData);
}



/**
 *  Set the color of the text
 *
 */
void set_text_color(SPI_HandleTypeDef *hspi, uint16_t text_color) {
	uint8_t TxData = 0;
	uint8_t RxData = 0;

	// Set Fore Color
	TxData = ((text_color & 0xf800) >> 11);
	SPI_write_register(hspi, 0x63, &TxData);

	TxData = ((text_color & 0x07e0) >> 5);
	SPI_write_register(hspi, 0x64, &TxData);

	TxData = text_color & 0x001f;
	SPI_write_register(hspi, 0x65, &TxData);

	// Set transparency flag
	TxData = text_color & 0x001f;
	SPI_read_register(hspi, 0x22, &RxData);
	RxData |= (1 << 6);
	SPI_write_data(hspi, &RxData);
}


/**
 *  Enlarge the text, 4x zoom is the max
 *
 */
void enlarge_text(SPI_HandleTypeDef *hspi, uint8_t zoom) {
	uint8_t actual_zoom = zoom - 1;
	uint8_t RxData = 0;

	if(actual_zoom > 3)
		actual_zoom = 3;

	SPI_read_register(hspi, 0x22 , &RxData);
	RxData &= ~(0xF);
	RxData |= actual_zoom << 2;
	RxData |= actual_zoom;
	SPI_write_data(hspi, &RxData);

}

/**
 * Initialize the display to our settings
 *
 */
void main_display_init(SPI_HandleTypeDef *hspi) {
	 initialize_display(hspi); // initialize display
	 display_on(hspi, true); // turn on display
	 GPIOX_on(hspi, true);  // Enable TFT - display enable tied to GPIOX
	 PWM1_config(hspi, true, RA8875_PWM_CLK_DIV1024); // turn on the backlight using PWM
	 PWM1_out(hspi, 255); // set blacklight to the highest
	 fill_screen(hspi, RA8875_BLACK);
	 text_mode(hspi); // Switch from graphics mode to text mode

}

void print_message(SPI_HandleTypeDef *hspi, char *msg, int height) {
	if(msg != NULL) {
		set_cursor(hspi, 15, 235 + height * next_line_height);
		text_write(hspi, msg, strlen(msg)); // Write the string to the display
	}
}

/**
 * Display number of people in store, number of people in Queue and output Messages:
 * using height 35 as next line
 */
void main_display_info(SPI_HandleTypeDef *hspi, int num_people_in_store, int num_people_in_queue, int max_capacity, char *msg1, char* msg2, char* msg3, char* msg4) {
	char buffer[64];
	char msg_header[] = "Messages:";

	// Basic setup, clear screen, set color and set text size
	clear_screen(hspi);
	set_text_color(hspi, RA8875_WHITE);
	enlarge_text(hspi, 2);

	sprintf(buffer, "Number of people in store: %d", num_people_in_store);
	set_cursor(hspi, 15, 20);
	text_write(hspi, buffer, strlen(buffer)); // Write the string to the display

	sprintf(buffer, "Number of people on queue: %d", num_people_in_queue);
	set_cursor(hspi, 15, 55);
	text_write(hspi, buffer, strlen(buffer)); // Write the string to the display

	sprintf(buffer, "Max Capacity allowed: %d", max_capacity);
	set_cursor(hspi, 15, 90);
	text_write(hspi, buffer, strlen(buffer)); // Write the string to the display

	set_cursor(hspi, 15, 200);
	text_write(hspi, msg_header, strlen(msg_header)); // Write the string to the display

	print_message(hspi, msg1, 0);
	print_message(hspi, msg2, 1);
	print_message(hspi, msg3, 2);
	print_message(hspi, msg4, 3);
}





