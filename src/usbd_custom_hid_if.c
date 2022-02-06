/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v1.0_Cube
  * @brief          : USB Device Custom HID interface file.
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
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */

#include "../configuration.h"

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define SECTOR_SIZE 1024
#define HID_RX_SIZE 64

/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

extern volatile uint8_t reset_mcu;

uint32_t current_Page = (USER_CODE_OFFSET / SECTOR_SIZE);
uint16_t currentPageOffset = 0;

static const uint8_t CMD_SIGNATURE[7] = {'B', 'T', 'L', 'D', 'C', 'M', 'D'};
/* Command: <Send next data pack> */
static uint8_t CMD_DATA_RECEIVED[8] = {'B', 'T', 'L', 'D', 'C', 'M', 'D', 2};

static uint8_t pageData[SECTOR_SIZE];

// memory page number calculation
#if USER_CODE_OFFSET < 0x10000
  #define USER_CODE_PAGE (USER_CODE_OFFSET / 0x4000)
#elif USER_CODE_OFFSET < 0x20000
  #define USER_CODE_PAGE 4
#else
  #define USER_CODE_PAGE (USER_CODE_OFFSET / 0x20000 + 4)
#endif

uint16_t erase_page = USER_CODE_PAGE;

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
	/* USER CODE BEGIN 0 */
	0x06, 0x00, 0xFF,//USAGE_PAGE (Vendor Defined Page 1)       // 3 B
	0x09, 0x00,      //USAGE (Undefined)                        // 5 B
	0xA1, 0x01,      //COLLECTION (Application)                 // 7 B

	0x75, 0x08,      //  REPORT_SIZE (8)                        // 9 B
	0x95, CUSTOM_HID_EPOUT_SIZE,      //  REPORT_COUNT (64)
	0x91, 0x03,			 //  OUTPUT (Cnst, Var, Abs)												   // 14B

	0x75, 0x08,      //  REPORT_SIZE (8)                        // 9 B
	0x95, CUSTOM_HID_EPIN_SIZE,      //  REPORT_COUNT (8)
	0x81, 0x03,			 //  INPUT (Cnst, Var, Abs)												   // 14B
	
	/* USER CODE END 0 */
	0xC0    /*     END_COLLECTION	             */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

static void ProcessPacket(const uint8_t *packet_data);
static void write_flash_sector(uint32_t currentPage);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
	CUSTOM_HID_ReportDesc_FS,
	CUSTOM_HID_Init_FS,
	CUSTOM_HID_DeInit_FS,
	CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return USBD_OK;
	/* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return USBD_OK;
	/* USER CODE END 5 */
}

/**
 * @brief  Manage the CUSTOM HID class events
 * @param  event_idx: Event index
 * @param  state: Event state
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */

  USBD_CUSTOM_HID_HandleTypeDef *hhid = (USBD_CUSTOM_HID_HandleTypeDef *)hUsbDeviceFS.pClassData;

  ProcessPacket(hhid->Report_buf);

  USBD_CUSTOM_HID_ReceivePacket(&hUsbDeviceFS);
  return USBD_OK;

  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */

static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
	return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}

/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

void ProcessPacket(const uint8_t *packet_data)
{
  if ((currentPageOffset % SECTOR_SIZE) == 0 && memcmp(packet_data, CMD_SIGNATURE, sizeof(CMD_SIGNATURE)) == 0)
  {
    switch (packet_data[7])
    {
    case 0x00: // Reset page indexes
      current_Page = (USER_CODE_OFFSET / SECTOR_SIZE);
      currentPageOffset = 0;
      erase_page = USER_CODE_PAGE;
      break;

    case 0x01: // Reset MCU
      if (currentPageOffset > 0)
      {
        // last page is partial, write last data
        write_flash_sector(current_Page);
      }
      reset_mcu = 1;
      break;
    }
  }
  else
  {
    memcpy(pageData + currentPageOffset, packet_data, HID_RX_SIZE);
    currentPageOffset += HID_RX_SIZE;
    if (currentPageOffset == SECTOR_SIZE)
    {
      write_flash_sector(current_Page);
      current_Page++;
      currentPageOffset = 0;
      USBD_CUSTOM_HID_SendReport_FS(CMD_DATA_RECEIVED, 8);
      memset(pageData, 0, sizeof(pageData));
    }
  }
}

void write_flash_sector(uint32_t currentPage)
{
  uint32_t pageAddress = FLASH_BASE + (currentPage * SECTOR_SIZE);
  uint32_t SectorError;

#ifdef LED_PIN
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
#endif
  FLASH_EraseInitTypeDef EraseInit;
  HAL_FLASH_Unlock();

  /* Sector to the erase the flash memory (16, 32, 48 ... kbytes) */
  if ((currentPage == 16) || (currentPage == 32) ||
      (currentPage == 48) || (currentPage == 64) ||
      (currentPage % 128 == 0))
  {
    EraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    /* Specify sector number. Starts from 0x08004000 */
    EraseInit.Sector = erase_page++;

    /* This is also important! */
    EraseInit.NbSectors = 1;
    HAL_FLASHEx_Erase(&EraseInit, &SectorError);
  }

  uint32_t dat;
  for (int i = 0; i < SECTOR_SIZE; i += 4)
  {
    dat = pageData[i + 3];
    dat <<= 8;
    dat += pageData[i + 2];
    dat <<= 8;
    dat += pageData[i + 1];
    dat <<= 8;
    dat += pageData[i];
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, pageAddress + i, dat);
  }
#ifdef LED_PIN
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
#endif
  HAL_FLASH_Lock();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
