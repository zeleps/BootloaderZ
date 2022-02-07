#ifndef __FLASH_H
#define __FLASH_H

#ifdef __cplusplus
  extern "C" {
#endif

uint8_t WriteFlash(uint32_t addr, uint8_t *buff, uint32_t len, uint8_t eraseIfNeeded);
uint8_t EraseFlashSector(uint32_t addr);

#ifdef __cplusplus
  }
#endif

#endif