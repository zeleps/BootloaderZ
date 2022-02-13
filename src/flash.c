#include "conditionals.h"

#include PLATFORM_H_MAIN
#include "flash.h"

#ifdef STM32F4XX

static uint8_t AddressToSector(uint32_t addr)
{
    uint8_t sector = (addr - FLASH_BASE) >> 14;
    return sector <= 4 ? sector : (sector >> 3) + 4;
}

#endif

static uint8_t IsSectorStart(uint32_t addr)
{
    #if ENABLED(FLASH_1K_PAGES)
        return (addr & 0x3ff) == 0;
    #elif ENABLED(FLASH_4X16_1X64_NX128_SECTORS)
        return (addr & 0x1ffff) == 0 || (addr <= 0x8010000 && (addr & 0x3fff) == 0);
    #endif
}

static uint8_t BeginWrite()
{
    TERN_(USE_LED, HAL_GPIO_WritePin(LED_PORT, LED_PIN, LED_ON));
    return HAL_FLASH_Unlock() == HAL_OK;
}

static uint8_t EndWrite()
{
    TERN_(USE_LED, HAL_GPIO_WritePin(LED_PORT, LED_PIN, LED_OFF));
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
    return IsSectorStart(addr) && BeginWrite() && EraseFlashSectorInt(addr) && EndWrite();
}

uint8_t WriteFlash(uint32_t addr, uint8_t *buff, uint32_t len, uint8_t eraseIfNeeded)
{
    if (!BeginWrite())
    {
        return 0;
    }

    if (eraseIfNeeded && IsSectorStart(addr))
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