/**
 * 
 *  Configuration macros
 * 
 */

#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include "macros.h"

/**
 * User code offset
 *
 * For DFU protocol, this is the first writable address. The value acts as a limit to prevent accidental overwriting
 * of the bootloader. This should also be in accordance with DFU_FLASH_DESCRIPTOR (DFU_FLASH_DESCRIPTOR is used to
 * limit writable memory from the host side (DFU tool), USER_CODE_OFFSET limits writing from the bootloader side).
 * 
 * For HID protocol, this is the firmware flashing base address (depends on firmware configuration).
 */
#define USER_CODE_OFFSET             0x8000

/**
 * USB Flashing Triggers
 */

/**
 * USB flashing magic number in RTC backup register
 * 
 * Specify one of the available RTC backup registers, in which the 
 * hosted firmware may store a magic number value and then reset the MCU.
 * If the bootloader reads the configured value from the configured register,
 * USB flashing mode will be entered.
 * Comment out the any of the following two macros to disable the feature.
 */

#define MAGIC_NUMBER_BKP_INDEX   LL_RTC_BKP_DR4     // RTC backup register to look for magic number. Number of available registers depends on the MCU type.
#define MAGIC_NUMBER_BKP_VALUE   0x424C             // magic number to look for

/**
 * Multi-clicking reset button trigger
 *
 * This trigger enters firmware flashing mode with multiple quick clicks on the reset button. Each click must occur within
 * MULTI_RESET_INTERVAL_MSEC msec from the previous. Uses the magic number feature, so magic number must be enabled and configured.
 * This is easier to trigger, but creates an additional MULTI_RESET_INTERVAL_MSEC delay to the system startup.
 * Comment out any of the following two macros to disable this feature.
 */

#define MULTI_RESET_INTERVAL_MSEC   500             // The maximum number of msec to wait between each reset before booting normally
#define MULTI_RESET_CLICKS          3               // The number of resets required to boot into firmware flashing mode

/**
 * USB flashing trigger pin
 * 
 * Specify a pin and its enabled state to trigger USB flashing mode.
 * Enabling the pin while resetting the board will enter USB flashing mode.
 * Specify port (GPIOx), pin number and the opposite of the default pin state.
 * E.g. to use the RepRap Display button as a trigger (PA15 -> gnd), set
 * PORT to A, PIN to 15 and STATE to PIN_RESET.
 * Comment out any of the following three macros to disable the feature.
 */

#define TRIGGER_PORT                 B              // port letter
#define TRIGGER_PIN                  2              // pin number
#define TRIGGER_STATE                PIN_SET        // PIN_SET / PIN_RESET

/**
 * USB flashing led indication
 *
 * Specify a pin to set when entering USB flashing mode. The pin will stay set
 * until data transfer begins. It will toggle while data is being written to memory
 * and it will reset when flashing completes and the board is reset.
 * E.g. to use a led connected to PA0 as an indicator, set PORT to A, PIN to 0, ON to PIN_SET.
 * Comment out any of the following three macros to disable the feature.
 */

#define LED_PORT                     C              // port letter
#define LED_PIN                      13             // pin number
#define LED_ON                       PIN_RESET      // PIN_SET / PIN_RESET

/**
 * USB Protocol
 * 
 * Uncomment one (and only one) of the following two macros:
 */

#define USB_PROTOCOL_DFU            // Fast, standardized, compatible. Will require a customized driver in windows (see: https://bit.ly/3gzkopp)
//#define USB_PROTOCOL_HID            // No specialized driver needed in windows. Slower, works only with HID-Flash tool (see: https://bit.ly/34JPXdt)

#ifdef USB_PROTOCOL_DFU             // Additional configuration for DFU protocol

    #define USB_DEVICE_USE_STM_ID         // 0483:df11 ID of the internal bootloader DFU device. Will appear as STM32 Bootloader.
    //#define USB_DEVICE_USE_STLINK_ID      // 0483:3748 ST-Link/v2 device ID, hardcoded into PlatformIO's DFU upload protocol. Ready to work with PlatformIO.
    //#define USB_DEVICE_USE_HID_ID         // 1209:BEBA standard HID-Flash compatible ID, may require uninstallation of DFU driver to switch back to HID.

    #if NONE(USB_DEVICE_USE_STM_ID, USB_DEVICE_USE_HID_ID, USB_DEVICE_USE_STLINK_ID)
        // define your own values
        #define USB_DEVICE_VENDOR_ID 0x1209
        #define USB_DEVICE_PRODUCT_ID 0xBEBA
    #endif
    
#endif

#endif