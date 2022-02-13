# Bootloader-Z

# IMPORTANT: This software, as any bootloader out there, might brick your device. USE AT YOUR OWN RISK.

This is a custom STM32F1xx/STM32F4xx bootloader that allows flashing firmware binaries via USB, using DFU or HID-flash protocols. The -ambitious- intention behind the project is to implmenet the intricacies of many more STM32 MCUs into a single project that can be easily downloaded, configured, compiled and flashed.

# Why?

Flashing new firmware with minimal interaction with the physical board is really convenient in some cases. In my case, it was flashing my 3d printer's BTT GTR 1.0 board through my OctoPi at the press of a button (two actually, reset + LCD encoder button), which I can now trigger via RPi's GPIO and flash at will without any manual interaction.

# The project

This bootloader is a work in progress, meaning it has not been tested widely, and there is still a lot to consider on hardware compatibility. Please consider this as something experimental, until you've made sure all aspects work on your specific board.

The project was inspired by [Serasidis/STM32_HID_Bootloader](https://github.com/Serasidis/STM32_HID_Bootloader) which originates from [bootsector/stm32-hid-bootloader](https://github.com/bootsector/stm32-hid-bootloader) and retains compatibility with the hid-flash tool.

The main change is adaptation to PlatformIO (the project should build and upload as is, provided you have an ST-Link/v2 device connected). It uses the original STM32Cube framework libraries instead of containing slightly modified parts of it. DFU protocol is implemented and tested on both STM32F1xx/4xx platforms. For the moment, compatibility with the older version of HID-Flash.exe is maintained, so the tool's sources are not included in the project.

Before you try this, be absolutely sure that you have a copy of the original bootloader of your board, *which you have successfully flashed back to the board without problems*.

# Configuration

Customizing the bootloader behavior is done by modifying the Configuration.h file. See file comments for details.

# Building and uploading

Use an ST-Link compatible device, properly connected to your board and the PC. Use PlatformIO's upload command to install. Resetting the board with BOOT0 pin set to 3.3V should also work, but this depends on your board and MCU (I will provide some details on this procedure later).

# Using

Enable the configured trigger and reset the board (in case of pin trigger, keep the pin enabled while resetting and until the USB device appears at the host. Depneding on which protocol you have enabled (DFU or HID), use [dfu-util.exe](https://github.com/Stefan-Schmidt/dfu-util) or [hid-flash.exe](https://github.com/Serasidis/STM32_HID_Bootloader/releases) cmd line tool to upload a firmware binary file manually, or if your firmware project is a PlatformIO project, simply configure `dfu` or `hid` as your preferred `upload-protocol` in the env section of your firmware's platformio.ini file, like in the following example:

```
#
# Bigtreetech GTR V1.0 (STM32F407IGT6 ARM Cortex-M4)
#
[env:BIGTREE_GTR_V1_0]
extends            = stm32_variant
board              = marlin_BigTree_GTR_v1
board_build.offset = 0x8000
build_flags        = ${stm32_variant.build_flags} -DSTM32F407IX
upload_protocol    = dfu
```

# Future development

More STM32 MCUs / boards supported.

So far, the project has been tested on the following boards:

STM32F4XX:

- BigTreeTech GTR 1.0 (STM32F407IGT6)

STM32F1XX (DFU only):

- HR0214-32 development board (STM32F103C8T6)
