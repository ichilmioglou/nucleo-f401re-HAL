#include "SEGGER_RTT.h"
#include "stm32f4xx_hal.h"

#define SECTOR1_START_ADRR 0x08004000
#define SECTOR1_LENGTH 16384

void flash_erase_sector_1() {
  HAL_FLASH_Unlock();
  FLASH_EraseInitTypeDef flash_erase = {0};
  flash_erase.Banks = FLASH_BANK_1;
  flash_erase.Sector = FLASH_SECTOR_1;
  flash_erase.NbSectors = 1;
  flash_erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  flash_erase.TypeErase = FLASH_TYPEERASE_SECTORS;
  uint32_t error = 0;
  if (HAL_FLASHEx_Erase(&flash_erase, &error) != HAL_OK) {
    SEGGER_RTT_printf(0, "ERROR ERASING!\n");
  }
}

void flash_fill_sector_1() {
  for (int i = 0; i < SECTOR1_LENGTH; i += 4) {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, SECTOR1_START_ADRR + i,
                          0x12345678) != HAL_OK) {
      SEGGER_RTT_printf(0, "ERROR WRITING!\n");
    }
  }
  HAL_FLASH_Lock();
}

void flash_print_sector_1() {
  uint32_t *psector1 = (uint32_t *)SECTOR1_START_ADRR;

  for (int i = 0; i < (SECTOR1_LENGTH / 4); i++) {
    SEGGER_RTT_printf(0, "ADDR= %08X   DATA=%08X  \n", &psector1[i],
                      psector1[i]);
  }
}

int main() {
  HAL_Init();

  flash_erase_sector_1();
  flash_fill_sector_1();
  flash_print_sector_1();
  for (;;) {
    HAL_Delay(1000);
  }

  return 0;
}