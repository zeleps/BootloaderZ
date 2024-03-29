#ifndef __CONDITIONALS_H
#define __CONDITIONALS_H

#include "../Configuration.h"

#if USER_CODE_OFFSET % 0x4000
#   error "Invalid USER_CODE_OFFSET must be on a valid page boundary."
#endif

#if USER_CODE_OFFSET < 0x4000
#   error "Invalid USER_CODE_OFFSET must be greater than 0x4000. First page is reserved for bootloader."
#endif

#if ENABLED(STM32F103xB)
#   define STM32F1XX
#   define PLATFORM stm32f1xx

#   define FLASH_PAGE_MAP PAGES_STM32F1XX
#   define FLASH_SIZE_DIVISOR 1024
#   define DFU_FLASH_END FLASH_BANK1_END

#   if !defined(DFU_FLASH_DESCRIPTOR) && !defined(AUTOCREATE_DFU_FLASH_DESCRIPTOR)
        #define DFU_FLASH_DESCRIPTOR "@Internal Flash /0x08000000/16*001Ka,48*001Kg,64*001Kg,64*001Kg,64*001Kg,64*001Kg,64*001Kg,64*001Kg,64*001Kg" // default for 512KB flash MCU, first 16K reserved for bootloader
#   endif
#elif ENABLED(STM32F407xx)
#   define STM32F4XX
#   define PLATFORM stm32f4xx

#   define FLASH_PAGE_MAP PAGES_STM32F4XX
#   define FLASH_SIZE_DIVISOR 16384
#   define DFU_FLASH_END FLASH_END

#   if !defined(DFU_FLASH_DESCRIPTOR) && !defined(AUTOCREATE_DFU_FLASH_DESCRIPTOR)
#       define DFU_FLASH_DESCRIPTOR "@Internal Flash /0x08000000/01*016Ka,03*016Kg,01*064Kg,07*128Kg,04*016Kg,01*064Kg,07*128Kg" // default for 2MB flash MCU, first sector reserved for bootloader
#   endif
#endif

#if ENABLED(STM32F1XX)
#   define ASSERT_VALID_FLASH_WRITE_ADDRESS(addr) (IS_FLASH_PROGRAM_ADDRESS(addr) && addr >= FLASH_BASE + USER_CODE_OFFSET)
#   define ASSERT_VALID_FLASH_READ_ADDRESS(addr) IS_FLASH_PROGRAM_ADDRESS(addr)
#else
#   define ASSERT_VALID_FLASH_WRITE_ADDRESS(addr) (addr >= FLASH_BASE + USER_CODE_OFFSET && addr <= FLASH_END)
#   define ASSERT_VALID_FLASH_READ_ADDRESS(addr) (addr >= FLASH_BASE && addr <= FLASH_END)
#endif

#define PLATFORM_H_MAIN STRINGIFY(PLATFORM.h)
#define PLATFORM_H_HAL STRINGIFY(CAT(PLATFORM, _hal.h))
#define PLATFORM_H_LL_RTC STRINGIFY(CAT(PLATFORM, _ll_rtc.h))
#define PLATFORM_H_LL_PWR STRINGIFY(CAT(PLATFORM, _ll_pwr.h))

#if defined(MAGIC_NUMBER_BKP_INDEX) && defined(MAGIC_NUMBER_BKP_VALUE)
#   define USE_MAGIC_NUMBER

#   if ENABLED(STM32F1XX)
#       define USE_RTC
#       define READ_MAGIC_NUMBER() HAL_RTCEx_BKUPRead(&hrtc, MAGIC_NUMBER_BKP_INDEX)
#       define WRITE_MAGIC_NUMBER(a) HAL_RTCEx_BKUPWrite(&hrtc, MAGIC_NUMBER_BKP_INDEX, a)
#   elif ENABLED(STM32F4XX)
#       define READ_MAGIC_NUMBER() LL_RTC_BAK_GetRegister(RTC, MAGIC_NUMBER_BKP_INDEX)
#       define WRITE_MAGIC_NUMBER(a) LL_RTC_BAK_SetRegister(RTC, MAGIC_NUMBER_BKP_INDEX, a)
#   endif

#   if defined(MULTI_RESET_INTERVAL_MSEC) && defined(MULTI_RESET_CLICKS)
#       define USE_MULTI_RESET

#       if MULTI_RESET_CLICKS < 2 || MULTI_RESET_CLICKS > 5
#           error "Number of multi reset clicks must be a number between 2 and 5, inclusive."
#       endif

#       if MAGIC_NUMBER_BKP_VALUE < MULTI_RESET_CLICKS
#           error "The magic value must be greater than or equal to the number of multi reset clicks."
#       endif
#   endif // MULTI_RESET
#endif // MAGIC_NUMBER

#if defined(MULTI_RESET_INTERVAL_MSEC) && defined(MULTI_RESET_CLICKS)
#   if !defined(USE_MAGIC_NUMBER)
#       error "Multiple reset click feature requires Magic Number."
#   endif
#endif

#if defined(TRIGGER_PORT) && defined(TRIGGER_PIN) && defined(TRIGGER_STATE)
#   define USE_TRIGGER_PIN
#endif

#if defined(LED_PORT) && defined(LED_PIN) && defined(LED_ON)
#   define USE_LED
#   if IS_STATE_SET(LED_ON)
#       define LED_OFF PIN_RESET
#       define LED_PULL GPIO_NOPULL
#       define INIT_LED()
#   else
#       define LED_OFF PIN_SET
#       define LED_PULL GPIO_PULLUP
#       define INIT_LED() SET_LED(LED_OFF)
#   endif
#   define SET_LED(a) HAL_GPIO_WritePin(PORT(LED_PORT), PIN(LED_PIN), STATE(a))
#else
#   define SET_LED(a)
#endif

#if NONE(USB_PROTOCOL_DFU, USB_PROTOCOL_HID)
#   error "A USB protocol must be defined. Please comment out one (and only one) of USB_PROTOCOL_DFU, USB_PROTOCOL_HID"
#elif BOTH(USB_PROTOCOL_DFU, USB_PROTOCOL_HID)
#   error "Only one USB protocol is supported. Please comment out one (and only one) of USB_PROTOCOL_DFU, USB_PROTOCOL_HID"
#endif

#if defined(USB_PROTOCOL_HID)
#   define USB_DEVICE_VENDOR_ID 0x1209
#   define USB_DEVICE_PRODUCT_ID 0xBEBA

#   define USB_PROTOCOL "HID"
#endif

#if defined(USB_PROTOCOL_DFU)

#   if MANY(USB_DEVICE_USE_STLINK_ID, USB_DEVICE_USE_HID_ID, USB_DEVICE_USE_STLINK_ID)
#       error "Only one USB device ID can be configured."
#   endif

#   if ENABLED(USB_DEVICE_USE_STM_ID)
#       define USB_DEVICE_VENDOR_ID 0x0483
#       define USB_DEVICE_PRODUCT_ID 0xdf11
#   elif ENABLED(USB_DEVICE_USE_STLINK_ID)
#       define USB_DEVICE_VENDOR_ID 0x0483
#       define USB_DEVICE_PRODUCT_ID 0x3748
#   elif ENABLED(USB_DEVICE_USE_HID_ID)
#       define USB_DEVICE_VENDOR_ID 0x1209
#       define USB_DEVICE_PRODUCT_ID 0xBEBA
#   endif

#   define USB_PROTOCOL "DFU"

#   if !defined(DFU_FLASH_DESCRIPTOR) && !defined(AUTOCREATE_DFU_FLASH_DESCRIPTOR)
#       error "DFU Flash descriptor is undefined."
#   endif

#   define DFU_FLASH_BASE FLASH_BASE
#   define DFU_USER_CODE_ADDRESS DFU_FLASH_BASE + USER_CODE_OFFSET

#endif // USB_PROTOCOL_DFU

#ifndef USB_DEVICE_VENDOR_ID
#   error "A vendor ID must be defined."
#endif

#ifndef USB_DEVICE_PRODUCT_ID
#   error "A product ID must be defined."
#endif

#endif // __CONDITIONALS_H