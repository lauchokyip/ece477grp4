/**
  ******************************************************************************
  * @file    stm32l476g_discovery_gyroscope.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    26-June-2015
  * @brief   This file provides a set of functions needed to manage the L3GD20
  *          MEMS accelerometer available on STM32L476G-Discovery board.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l476g_discovery_gyroscope.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L476G_DISCOVERY
  * @{
  */

/** @defgroup STM32L476G_DISCOVERY_GYROSCOPE STM32L476G-DISCOVERY GYROSCOPE
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/** @defgroup STM32L476G_DISCOVERY_GYROSCOPE_Private_Types Private Types
  * @{
  */
/**
  * @}
  */

/* Private defines ------------------------------------------------------------*/
/** @defgroup STM32L476G_DISCOVERY_GYROSCOPE_Private_Constants Private Constants
  * @{
  */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup STM32L476G_DISCOVERY_GYROSCOPE_Private_Macros Private Macros
  * @{
  */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup STM32L476G_DISCOVERY_GYROSCOPE_Private_Variables Private Variables
  * @{
  */
static GYRO_DrvTypeDef *GyroscopeDrv;

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup STM32L476G_DISCOVERY_GYROSCOPE_Private_FunctionPrototypes Private Functions
  * @{
  */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup STM32L476G_DISCOVERY_GYROSCOPE_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize Gyroscope.
  * @retval GYRO_OK or GYRO_ERROR
  */
uint8_t BSP_GYRO_Init(void)
{  
  uint8_t ret = GYRO_ERROR;
  uint16_t ctrl = 0x0000;
  GYRO_InitTypeDef L3GD20_InitStructure;
  GYRO_FilterConfigTypeDef L3GD20_FilterStructure={0,0};

  if((L3gd20Drv.ReadID() == I_AM_L3GD20) || (L3gd20Drv.ReadID() == I_AM_L3GD20_TR))
  {
    /* Initialize the gyroscope driver structure */
    GyroscopeDrv = &L3gd20Drv;

    /* Configure Mems : data rate, power mode, full scale and axes */
    L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
    L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
    L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
    L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
    L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
    L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
    L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_500; 

    /* Configure MEMS: data rate, power mode, full scale and axes */
    ctrl = (uint16_t) (L3GD20_InitStructure.Power_Mode | L3GD20_InitStructure.Output_DataRate | \
                       L3GD20_InitStructure.Axes_Enable | L3GD20_InitStructure.Band_Width);

    ctrl |= (uint16_t) ((L3GD20_InitStructure.BlockData_Update | L3GD20_InitStructure.Endianness | \
                        L3GD20_InitStructure.Full_Scale) << 8);

    /* Initialize component */
    GyroscopeDrv->Init(ctrl);
  
    L3GD20_FilterStructure.HighPassFilter_Mode_Selection =L3GD20_HPM_NORMAL_MODE_RES;
    L3GD20_FilterStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_0;

    ctrl = (uint8_t) ((L3GD20_FilterStructure.HighPassFilter_Mode_Selection |\
                       L3GD20_FilterStructure.HighPassFilter_CutOff_Frequency));

    /* Configure component filter */
    GyroscopeDrv->FilterConfig(ctrl) ;
  
    /* Enable component filter */
    GyroscopeDrv->FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);
    
    ret = GYRO_OK;
  }
  else
  {
    ret = GYRO_ERROR;
  }
  
  return ret;
}


/**
  * @brief  DeInitialize Gyroscope.
  * @retval None
  */
void BSP_GYRO_DeInit(void)
{
  GYRO_IO_DeInit();  
}


/**
  * @brief  Put Gyroscope in low power mode.
  * @retval None
  */
void BSP_GYRO_LowPower(void)
{
  uint16_t ctrl = 0x0000;
  GYRO_InitTypeDef L3GD20_InitStructure;
  
  /* configure only Power_Mode field */ 
  L3GD20_InitStructure.Power_Mode = L3GD20_MODE_POWERDOWN;
  
  ctrl = (uint16_t) (L3GD20_InitStructure.Power_Mode);
  
  /* Set component in low-power mode */
  GyroscopeDrv->LowPower(ctrl);


}

/**
  * @brief  Read ID of Gyroscope component.
  * @retval ID
  */
uint8_t BSP_GYRO_ReadID(void)
{
  uint8_t id = 0x00;

  if(GyroscopeDrv->ReadID != NULL)
  {
    id = GyroscopeDrv->ReadID();
  }  
  return id;
}

/**
  * @brief  Reboot memory content of Gyroscope.
  * @retval None
  */
void BSP_GYRO_Reset(void)
{
  if(GyroscopeDrv->Reset != NULL)
  {
    GyroscopeDrv->Reset();
  }  
}

/**
  * @brief  Configure Gyroscope interrupts (INT1 or INT2).
  * @param  pIntConfig: pointer to a GYRO_InterruptConfigTypeDef 
  *         structure that contains the configuration setting for the L3GD20 Interrupt.
  * @retval None
  */
void BSP_GYRO_ITConfig(GYRO_InterruptConfigTypeDef *pIntConfig)
{  
  uint16_t interruptconfig = 0x0000;

  if(GyroscopeDrv->ConfigIT != NULL)
  {
    /* Configure latch Interrupt request and axe interrupts */                   
    interruptconfig |= ((uint8_t)(pIntConfig->Latch_Request| \
                                  pIntConfig->Interrupt_Axes) << 8);

    interruptconfig |= (uint8_t)(pIntConfig->Interrupt_ActiveEdge);

    GyroscopeDrv->ConfigIT(interruptconfig);
  }
}

/**
  * @brief  Enable Gyroscope interrupts (INT1 or INT2).
  * @param  IntPin: Interrupt pin 
  *      This parameter can be: 
  *        @arg L3GD20_INT1
  *        @arg L3GD20_INT2
  * @retval None
  */
void BSP_GYRO_EnableIT(uint8_t IntPin)
{
  if(GyroscopeDrv->EnableIT != NULL)
  {
    GyroscopeDrv->EnableIT(IntPin);
  }
}

/**
  * @brief  Disable Gyroscope interrupts (INT1 or INT2).
  * @param  IntPin: Interrupt pin 
  *      This parameter can be: 
  *        @arg L3GD20_INT1
  *        @arg L3GD20_INT2
  * @retval None
  */
void BSP_GYRO_DisableIT(uint8_t IntPin)
{
  if(GyroscopeDrv->DisableIT != NULL)
  {
    GyroscopeDrv->DisableIT(IntPin);
  }
}

/**
  * @brief  Get XYZ angular acceleration from the Gyroscope.
  * @param  pfData: pointer on floating array         
  * @retval None
  */
void BSP_GYRO_GetXYZ(float* pfData)
{
  if(GyroscopeDrv->GetXYZ!= NULL)
  {
    GyroscopeDrv->GetXYZ(pfData);
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
