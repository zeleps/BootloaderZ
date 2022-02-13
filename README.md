# Bootloader-Z

# IMPORTANT: This software, as any bootloader out there, might brick your device. USE AT YOUR OWN RISK.

This is a custom STM32F4xx bootloader that allows flashing firmware binaries via USB.

The project is a modified version of [Serasidis/STM32_HID_Bootloader](https://github.com/Serasidis/STM32_HID_Bootloader) which originates from [bootsector/stm32-hid-bootloader](https://github.com/bootsector/stm32-hid-bootloader).

The main change is adaptation to PlatformIO (the project should build and upload as is, provided you have an ST-Link/v2 device connected). It uses the original STM32Cube framework libraries instead of containing slightly modified parts of it. Some possible race conditions are also averted. For the moment, compatibility with the older version of HID-Flash.exe is maintained, so the tool's sources are not included in the project.

Before you try this, be absolutely sure that you have a copy of the original bootloader of your board, which you have successfully flashed back to the board without problems.

# Configuration

Customizing the bootloader behavior is done by modifying the Configuration.h file. See file comments for details.

# Building and uploading

Use an ST-Link compatible device, properly connected to your board and the PC. Use PlatformIO's upload command to install.

# Using

Enable the configured trigger and reset the board (in case of pin trigger, keep the pin enabled while resetting and until the USB device appears at the host. Use [hid-flash.exe](https://github.com/Serasidis/STM32_HID_Bootloader/releases) cmd line tool to upload a firmware binary file manually, or if your firmware project is a PlatformIO project, simply configure `hid` as your preferred `upload-protocol` in the env section of your firmware's platformio.ini file, like in the following example:

```
#
# Bigtreetech GTR V1.0 (STM32F407IGT6 ARM Cortex-M4)
#
[env:BIGTREE_GTR_V1_0]
extends            = stm32_variant
board              = marlin_BigTree_GTR_v1
board_build.offset = 0x8000
build_flags        = ${stm32_variant.build_flags} -DSTM32F407IX
upload_protocol   = hid
```

# Future development

Hopefully, a DFU flashing mode will be available soon and more MCUs will be supported.
