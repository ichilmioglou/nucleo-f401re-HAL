#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

extern "C" {
void SysTick_Handler() { HAL_IncTick(); }
}