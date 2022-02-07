/**
 * 
 *  Configuration macros
 * 
 */

#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H


#define USER_CODE_OFFSET             0x8000 // <USER CODE> flash start address (*must* match with firmware)

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
 */

#define HID_MAGIC_NUMBER_BKP_INDEX   LL_RTC_BKP_DR4 // RTC backup register to look for magic number. Number of available registers depends on the MCU type.
#define HID_MAGIC_NUMBER_BKP_VALUE   0x424C // magic number to look for

/**
 * USB flashing trigger pin
 * 
 * Specify a pin and its enabled state to trigger USB flashing mode.
 * Enabling the pin while resetting the board will enter USB flashing mode.
 * Specify port (GPIOx), pin number and the opposite of the default pin state.
 * E.g. to use the LCD button as a trigger (PA15 -> gnd), set
 * PORT to GPIOA, PIN to 15 and enabled to GPIO_PIN_RESET.
 */

#define TRIGGER_PORT                 GPIOA
#define TRIGGER_PIN                  GPIO_PIN_15 
#define TRIGGER_STATE                GPIO_PIN_RESET

/**
 * USB flashing led indication
 * 
 * Specify a pin to set when entering USB flashing mode. The pin will stay set
 * until data transfer begins. It will toggle while data is being written to memory
 * and it will reset when flashing completes and the board is reset.
 * E.g. to use a led connected to PA0 as an indicator, set PORT to GPIOA, PIN to 0.
 */

#define LED_PORT                     GPIOA
#define LED_PIN                      GPIO_PIN_0

#endif