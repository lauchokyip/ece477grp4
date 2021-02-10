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
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
	  BSP_LCD_GLASS_DisplayString("Hello ");
	  HAL_Delay(1000);
	  BSP_LCD_GLASS_DisplayString("      ");
	  HAL_Delay(1000);
	  BSP_LCD_GLASS_DisplayString("World!");
	  HAL_Delay(1000);
	  BSP_LCD_GLASS_DisplayString("      ");
	  HAL_Delay(1000);
  }
  /* USER CODE END 3 */
}
