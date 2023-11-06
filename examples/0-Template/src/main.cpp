#include "SEGGER_RTT.h"
#include "stm32f4xx_hal.h"

int main() {
  HAL_Init();
  for (;;) {
    SEGGER_RTT_printf(0, "Hello World\n");
    HAL_Delay(1000);
  }

  return 0;
}