#include <stm32f4xx.h>
#include "flash.h"

#include "../configuration.h"

static uint8_t AddressToSector(uint32_t addr)
{
    uint8_t sector = (addr - FLASH_BASE) >> 14;
    return sector <= 4 ? sector : (sector >> 3) + 4;
}

static uint8_t IsSectorStart(uint32_t addr)
{
    return (addr & 0x1ffff) == 0 || (addr <= 0x8010000 && (addr & 0x3fff) == 0);
}

static void BeginWrite()
{
    HAL_FLASH_Unlock();
#ifdef LED_PIN
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
#endif
}

static void EndWrite()
{
#ifdef LED_PIN
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
#endif
    HAL_FLASH_Lock();
}

static uint8_t EraseFlashSectorInt(uint32_t addr)
{
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t sectorError;

    eraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    eraseInit.Sector = AddressToSector(addr);
    eraseInit.NbSectors = 1;

    HAL_FLASHEx_Erase(&eraseInit, &sectorError);

    return sectorError == 0xffffffff;
}

uint8_t EraseFlashSector(uint32_t addr)
{
    if (!IsSectorStart(addr))
    {
        return 0;
    }

    BeginWrite();

    uint8_t r = EraseFlashSectorInt(addr);

    EndWrite();

    return r;
}

uint8_t WriteFlash(uint32_t addr, uint8_t *buff, uint32_t len, uint8_t eraseIfNeeded)
{
    BeginWrite();

    if (eraseIfNeeded && IsSectorStart(addr))
    {
        EraseFlashSectorInt(addr);
    }

    HAL_StatusTypeDef result = HAL_OK;

    for (int i = 0; i < len; i += sizeof(uint32_t))
    {
        result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i, *(uint32_t *)(buff + i));

        if (result != HAL_OK)
        {
            break;
        }
    }

    EndWrite();

    return result == HAL_OK;
}
