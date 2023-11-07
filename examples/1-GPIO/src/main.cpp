#include "SEGGER_RTT.h"
#include "stm32f4xx_hal.h"
/**
 * GPIO led blink example using HAL
 *
 * Use pin PA9
 */

int main() {
  HAL_Init();
  for (;;) {
    SEGGER_RTT_printf(0, "Led Toggle\n");
    HAL_Delay(1000);
  }

  return 0;
}