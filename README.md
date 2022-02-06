# Bootloader-Z

# IMPORTANT: This software, as any bootloader out there, might brick your device. USE AT YOUR OWN RISK.

This is a custom STM32F4xx bootloader that allows flashing firmware binaries via USB.

The project is a modified version of [Serasidis/STM32_HID_Bootloader](https://github.com/Serasidis/STM32_HID_Bootloader) which originates from [bootsector/stm32-hid-bootloader](https://github.com/bootsector/stm32-hid-bootloader).

The main change is adaptation to PlatformIO (the project should build and upload as is, provided you have an ST-Link/v2 device connected). It uses the original STM32Cube framework libraries instead of containing slightly modified parts of it. Some possible race conditions are also averted. For the moment, compatibility with the older version of HID-Flash.exe is maintained, so the tool's sources are not included in the project.

Before you try this, be absolutely sure that you have a copy of the original bootloader of your board, which you have successfully flashed back to the board without problems.

Detailed instructions for use will follow soon.
