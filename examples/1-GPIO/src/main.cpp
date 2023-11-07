#include "SEGGER_RTT.h"
#include "stm32f4xx_hal.h"
/**
 * GPIO led blink example using HAL
 *
 * Use pin PA9
 */

void gpio_init() {
  if (__HAL_RCC_GPIOA_IS_CLK_DISABLED())
    __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef gpio = {0};
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pin = GPIO_PIN_9;
  HAL_GPIO_Init(GPIOA, &gpio);
}

int main() {
  HAL_Init();
  gpio_init();
  for (;;) {
    SEGGER_RTT_printf(0, "Led Toggle\n");
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
    HAL_Delay(1000);
  }

  return 0;
}