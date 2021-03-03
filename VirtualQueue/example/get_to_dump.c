/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "lcd.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "retarget.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t rbuf[2000];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  BSP_LCD_GLASS_Init();

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LCD_Init();
  MX_UART4_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //BSP_LCD_GLASS_DisplayString("Hello");
  RetargetInit(&huart2);
  printf("\r\nStarting\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  esp8266Init();
  printf("set up\r\n");
  uint8_t send[] = "AT+CIPSEND=67\r\n";
  uint8_t data[] = "GET http://www.ptsv2.com/t/q25ox-1614529517/post?a=1 HTTP/1.1\r\n\r\n\r\n";
  HAL_UART_Transmit(&huart4, send, sizeof(send)/sizeof(uint8_t), 100);
  HAL_UART_Receive(&huart4, rbuf, 2000, 500);
  printf("%s\r\n", rbuf);
  clearRBuf();
  HAL_UART_Transmit(&huart4, data, sizeof(data)/sizeof(uint8_t), 100);
  HAL_UART_Receive(&huart4, rbuf, 2000, 2000);
  printf("%s\r\n", rbuf);
  clearRBuf();
  while (1)
  {

  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void clearRBuf() {
	int i;
	for (i = 0; i < 2000; ++i) {
		rbuf[i] = '\0';
	}
}

void esp8266Init() {
	// reset
	uint8_t reset[] = "AT+RST\r\n";
	HAL_UART_Transmit(&huart4, reset, sizeof(reset)/sizeof(uint8_t), 100);
	HAL_UART_Receive(&huart4, rbuf, 2000, 5000);
	printf("%s\r\n", rbuf);
	clearRBuf();
	// set mode to station/client
	uint8_t mode[] = "AT+CWMODE=1\r\n";
	HAL_UART_Transmit(&huart4, mode, sizeof(mode)/sizeof(uint8_t), 100);
	HAL_UART_Receive(&huart4, rbuf, 2000, 500);
	printf("%s\r\n", rbuf);
	clearRBuf();
	// set connections to 1 at a time
	uint8_t numcons[] = "AT+CIPMUX=0\r\n";
	HAL_UART_Transmit(&huart4, numcons, sizeof(numcons)/sizeof(uint8_t), 100);
	HAL_UART_Receive(&huart4, rbuf, 2000, 500);
	printf("%s\r\n", rbuf);
	clearRBuf();
	// connect to given wifi
	/*uint8_t connect[] = "AT+CWJAP=\"TEST-HOTSPOT\",\"65c9O21=\"\r\n";
	HAL_UART_Transmit(&huart4, connect, sizeof(connect)/sizeof(uint8_t), 100);
	HAL_UART_Receive(&huart4, rbuf, 2000, 5000);
	printf("%s\r\n", rbuf);*/
	clearRBuf();
	// start tcp connection to server
	uint8_t start[] = "AT+CIPSTART=\"TCP\",\"www.ptsv2.com\",80\r\n";
	HAL_UART_Transmit(&huart4, start, sizeof(start)/sizeof(uint8_t), 100);
	HAL_UART_Receive(&huart4, rbuf, 2000, 10000);
	printf("%s\r\n", rbuf);
	clearRBuf();
}

void sendCommand(uint8_t msg[], int size, int response_size) {
	//int rsize = size + response_length;
	HAL_UART_Transmit(&huart4, msg, size, 100);
	HAL_UART_Receive_IT(&huart4, rbuf, size + response_size);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//HAL_UART_Receive_IT(&huart4, &rbyte, 1);
	printf("%s", rbuf);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
