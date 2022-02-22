#include "conditionals.h"

#include PLATFORM_H_MAIN
#include "flash.h"

#include "flash_sector_start.h"

#ifdef STM32F4XX

static uint8_t AddressToSector(uint32_t addr)
{
    uint8_t sector = (addr - FLASH_BASE) / FLASH_SIZE_DIVISOR;
    return ADDRESS_TO_SECTOR(sector);
}

#endif

static uint8_t BeginWrite()
{
    SET_LED(LED_ON);
    return HAL_FLASH_Unlock() == HAL_OK;
}

static uint8_t EndWrite()
{
    SET_LED(LED_OFF);
    return HAL_FLASH_Lock() == HAL_OK;
}

static uint8_t EraseFlashSectorInt(uint32_t addr)
{
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t sectorError;

#if ENABLED(STM32F1XX)
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.PageAddress = addr;
    eraseInit.NbPages = 1;
#elif ENABLED(STM32F4XX)
    eraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    eraseInit.Sector = AddressToSector(addr);
    eraseInit.NbSectors = 1;
#endif

    HAL_FLASHEx_Erase(&eraseInit, &sectorError);

    if (sectorError != 0xffffffff)
    {
        return 0;
    }
    return 1;
}

uint8_t EraseFlashSector(uint32_t addr)
{
    return (IS_SECTOR_START_ADDRESS(addr)) && BeginWrite() && EraseFlashSectorInt(addr) && EndWrite();
}

uint8_t WriteFlash(uint32_t addr, uint8_t *buff, uint32_t len, uint8_t eraseIfNeeded)
{
    if (!BeginWrite())
    {
        return 0;
    }

    if (eraseIfNeeded && (IS_SECTOR_START_ADDRESS(addr)))
    {
        if (!EraseFlashSectorInt(addr))
        {
            return 0;
        }
    }

    for (int i = 0; i < len; i += sizeof(uint32_t))
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i, *(uint32_t *)(buff + i)) != HAL_OK)
        {
            return 0;
        }
    }

    return EndWrite();
}