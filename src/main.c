
/*******************************************************************************
 * 
  * Bootloader-Z
  * 
  * A bootloader for conditional firmware flashing via USB for STM32 MCUs
  * 
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include PLATFORM_H_HAL
#include "usb_device.h"
#include PLATFORM_H_LL_RTC
#include PLATFORM_H_LL_PWR

#include "clock.h"

#include "flash.h"

/* Private variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

typedef void (*funct_ptr)(void);

/**
 * HID Flashing variables
 */

volatile uint8_t reset_mcu = 0;

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void WriteMagicNumber(uint16_t);

#if ENABLED(USE_RTC)

    RTC_HandleTypeDef hrtc;

static void InitRTC()
{
  hrtc.Instance = RTC;
  //hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  //hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;

  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
}

#endif

typedef void (*pFunction)(void);

/**
 * @brief  The application entry point.
 *
 * @retval None
 */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  MX_GPIO_Init();

#ifdef USE_MAGIC_NUMBER
  TERN_(USE_RTC, InitRTC());
  uint32_t magic_val = READ_MAGIC_NUMBER();

#if defined(USE_MULTI_RESET)
  if (magic_val != MAGIC_NUMBER_BKP_VALUE)
  {
    WriteMagicNumber(magic_val == 0 ? MAGIC_NUMBER_BKP_VALUE - MULTI_RESET_CLICKS + 2 : magic_val + 1);

    HAL_Delay(MULTI_RESET_INTERVAL_MSEC);

    WriteMagicNumber(0);
  }
#endif

#endif

  /* In case of incoming magic number or <TRIGGER_PIN> is LOW,
    jump to HID bootloader */
  if (TERN1(USE_MAGIC_NUMBER, magic_val != MAGIC_NUMBER_BKP_VALUE) &&
      TERN1(USE_TRIGGER_PIN, HAL_GPIO_ReadPin(PORT(TRIGGER_PORT), PIN(TRIGGER_PIN)) != STATE(TRIGGER_STATE)))
  {
    SET_LED(LED_OFF);

    pFunction Jump_To_Application;
    uint32_t JumpAddress;
    
    JumpAddress = *(__IO uint32_t*) (FLASH_BASE + USER_CODE_OFFSET + 4UL);
    Jump_To_Application = (pFunction) JumpAddress;
    __set_MSP(*(uint32_t *) (FLASH_BASE + USER_CODE_OFFSET));
    Jump_To_Application(); 
  }

  SET_LED(LED_ON);

#ifdef USE_MAGIC_NUMBER
  /* Reset the magic number backup memory */
  if (magic_val == MAGIC_NUMBER_BKP_VALUE)
  {
    WriteMagicNumber(0);
  }
#endif
  
  /* USER CODE END SysInit */
  
  /* Initialize all configured peripherals */

#if ANY(USE_MAGIC_NUMBER, USE_TRIGGER_PIN)
  MX_USB_DEVICE_Init();
#endif

  /* Infinite loop */
  while (!reset_mcu)
    ;

  HAL_Delay(100);
  HAL_NVIC_SystemReset();

}

static void WriteMagicNumber(uint16_t val)
{
#ifdef USE_RTC
  static uint8_t first_time = 1;

  if (first_time == 1)
  {
    first_time = 0;
    HAL_RTCEx_DeactivateTamper(&hrtc, RTC_TAMPER_1);
    __HAL_RTC_TAMPER_CLEAR_FLAG(&hrtc, RTC_FLAG_TAMP1F);
  }
#endif

  LL_PWR_EnableBkUpAccess();
  WRITE_MAGIC_NUMBER(val);
  LL_PWR_DisableBkUpAccess();
}

/* Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Configure GPIO pin : TRIGGER */
#if ENABLED(USE_TRIGGER_PIN)
  PORT_ENABLE(TRIGGER_PORT);
  GPIO_InitStruct.Pin =  PIN(TRIGGER_PIN);
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PORT(TRIGGER_PORT), &GPIO_InitStruct);
#endif

  /* Configure GPIO pin : LED */
#if ENABLED(USE_LED)
  PORT_ENABLE(LED_PORT);
  GPIO_InitStruct.Pin = PIN(LED_PIN);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull =  LED_PULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PORT(LED_PORT), &GPIO_InitStruct);
  INIT_LED();
#endif
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  __disable_irq();
  while (1)
    ;
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name
   * and line number, tex: printf("Wrong parameters value: file
   * %s on line %d\r\n", file, line)
   */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
