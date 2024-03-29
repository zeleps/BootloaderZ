/**
  ******************************************************************************
  * @file    usbd_customhid_if_template.h
  * @author  MCD Application Team
  * @brief   Header for usbd_customhid_if_template.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CUSTOMHID_IF_H
#define __USBD_CUSTOMHID_IF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_customhid.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops;

int8_t CUSTOM_HID_SendReport();

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CUSTOMHID_IF_TEMPLATE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
