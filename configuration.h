/**
 * 
 *  Configuration macros
 * 
 */

#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H


#define USER_CODE_OFFSET 0x8000 // <USER CODE> flash start address (*must* match with firmware)

#define HID_MAGIC_NUMBER_BKP_INDEX LL_RTC_BKP_DR4 // Data register to look for magic number
#define HID_MAGIC_NUMBER_BKP_VALUE 0x424C // magic number

/**
 * HID flashing trigger pin
 * 
 * Specify a pin and its enabled state to trigger USB HID function.
 * Enabling the pin while resetting the board will enter HID flashing mode.
 * Specify port (GPIOx), pin number and the opposite of the default pin state.
 * E.g. to use the LCD button as a trigger (PA15 -> gnd), set
 * PORT to GPIOA, PIN to 15 and enabled to GPIO_PIN_RESET.
 */

#define TRIGGER_PORT GPIOA
#define TRIGGER_PIN GPIO_PIN_15 
#define TRIGGER_STATE GPIO_PIN_RESET

/**
 * HID flashing led indication
 * 
 * Specify a pin to set when entering HID flash mode. The pin will stay set
 * until data transfer begins. It will toggle while data is being written to memory
 * and it will reset when flashing completes and the board is reset.
 * E.g. to use a led connected to PA0 as an indicator, set PORT to GPIOA, PIN to 0.
 */

#define LED_PORT GPIOA
#define LED_PIN GPIO_PIN_0

#endif