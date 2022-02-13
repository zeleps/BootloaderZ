/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f1xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /** DISABLE: JTAG-DP Disabled and SW-DP Disabled
  */
  //__HAL_AFIO_REMAP_SWJ_DISABLE();

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void HAL_MspDeInit(void)
{
  __HAL_RCC_PWR_CLK_DISABLE();
  __HAL_RCC_AFIO_CLK_DISABLE();
}

#if ENABLED(USE_MAGIC_NUMBER)

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  if (hrtc->Instance == RTC)
  {
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* Peripheral clock enable */
    __HAL_RCC_RTC_ENABLE();
    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
  }
}

/**
 * @brief RTC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  if (hrtc->Instance == RTC)
  {
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt DeInit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  }
}

#endif