/**
  ******************************************************************************
  * @file    usbd_customhid_if_template.c
  * @author  MCD Application Team
  * @brief   USB Device Custom HID interface file.
  *        This template should be copied to the user folder, renamed and customized
  *          following user needs.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "usbd_customhid_if.h"
#include "usb_device.h"
#include "main.h"
#include "flash.h"

#include "../configuration.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static int8_t CUSTOM_HID_Init(void);
static int8_t CUSTOM_HID_DeInit(void);
static int8_t CUSTOM_HID_OutEvent(uint8_t event_idx, uint8_t state);
static uint8_t ProcessPacket(const uint8_t *packetData);

/* Private variables ---------------------------------------------------------*/
#define HID_RX_SIZE 64
#define BUFFER_SIZE 1024

volatile uint32_t currentAddress = FLASH_BASE + USER_CODE_OFFSET;
volatile uint16_t bufferIdx = 0;

#if defined(__ICCARM__) /* IAR Compiler */
#pragma data_alignment = 4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN uint8_t buffer[BUFFER_SIZE] __ALIGN_END;

static uint8_t CMD_SIGNATURE[8] = {'B', 'T', 'L', 'D', 'C', 'M', 'D', 2};

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
    {
        0x06, 0x00, 0xFF, // USAGE_PAGE (Vendor Defined Page 1)       // 3 B
        0x09, 0x00,       // USAGE (Undefined)                        // 5 B
        0xA1, 0x01,       // COLLECTION (Application)                 // 7 B

        0x75, 0x08,                  //  REPORT_SIZE (8)              // 9 B
        0x95, CUSTOM_HID_EPOUT_SIZE, //  REPORT_COUNT (64)
        0x91, 0x03,                  //  OUTPUT (Cnst, Var, Abs)			// 14B

        0x75, 0x08,                 //  REPORT_SIZE (8)               // 9 B
        0x95, CUSTOM_HID_EPIN_SIZE, //  REPORT_COUNT (8)
        0x81, 0x03,                 //  INPUT (Cnst, Var, Abs)				// 14B

        0xC0 /*     END_COLLECTION	             */
};

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops =
    {
        CUSTOM_HID_ReportDesc,
        CUSTOM_HID_Init,
        CUSTOM_HID_DeInit,
        CUSTOM_HID_OutEvent,
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  CUSTOM_HID_Init
  *         Initializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init(void)
{
  return (0);
}

/**
  * @brief  CUSTOM_HID_DeInit
  *         DeInitializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit(void)
{
  /*
     Add your deinitialization code here
  */
  return (0);
}

#ifdef STM32F1XX
uint8_t USBD_CUSTOM_HID_ReceivePacket(USBD_HandleTypeDef *pdev)
{
  return USBD_OK;
}
#endif

/**
  * @brief  CUSTOM_HID_Control
  *         Manage the CUSTOM HID class events
  * @param  event_idx: event index
  * @param  state: event state
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent(uint8_t event_idx, uint8_t state)
{
  UNUSED(event_idx);
  UNUSED(state);

  USBD_CUSTOM_HID_HandleTypeDef *hhid = (USBD_CUSTOM_HID_HandleTypeDef *)USBD_Device.pClassData;

  if (ProcessPacket(hhid->Report_buf))
  {
    USBD_CUSTOM_HID_ReceivePacket(&USBD_Device); // Start next USB packet transfer once data processing is completed
  }
  
  return (0);
}

int8_t CUSTOM_HID_SendReport()
{
  return USBD_CUSTOM_HID_SendReport(&USBD_Device, CMD_SIGNATURE, 8);
}

static uint8_t ProcessPacket(const uint8_t *packetData)
{
  if (memcmp(packetData, CMD_SIGNATURE, sizeof(CMD_SIGNATURE) - 1) == 0)
  {
    switch (packetData[7])
    {
      case 0x00: // Reset indexes
        currentAddress = FLASH_BASE + USER_CODE_OFFSET;
        bufferIdx = 0;
        return 1;

      case 0x01: // Reset MCU
        if (bufferIdx > 0) // last page is partial, write last data
        {
          WriteFlash(currentAddress, buffer, bufferIdx, 1);
        }
        reset_mcu = 1;
        return 0; // receiving next packet is unnecessary; board will reset
    }
  }

  memcpy(buffer + bufferIdx, packetData, HID_RX_SIZE);
  bufferIdx += HID_RX_SIZE;

  if (bufferIdx == BUFFER_SIZE) 
  {
    WriteFlash(currentAddress, buffer, bufferIdx, 1);
    currentAddress += BUFFER_SIZE;
    bufferIdx = 0;
    CUSTOM_HID_SendReport();
    return 1; // flash writing and next packet reception is triggered in main loop
  }

  return 1;
}

  /************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
