#include "SEGGER_RTT.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_exti.h"
#include "stm32f4xx_hal_gpio.h"
/**
 * gpio falling edge interrupt using HAL
 *
 * use pin PA9
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == GPIO_PIN_9) {
    SEGGER_RTT_printf(0, "Interrupt occurred on pin PA9\n");
  }
}

extern "C" {
void EXTI9_5_IRQHandler(void) { HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9); }
}

void gpio_init() {
  if (__HAL_RCC_GPIOA_IS_CLK_DISABLED())
    __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef gpio = {0};
  gpio.Mode = GPIO_MODE_IT_FALLING;
  gpio.Pin = GPIO_PIN_9;
  gpio.Pull = GPIO_PULLUP;

  HAL_GPIO_Init(GPIOA, &gpio);
}

void interrupt_init() {
  // Enable and set EXTI Line9 Interrupt to the lowest priority
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0x0F, 0x00);
  NVIC_EnableIRQ(EXTI9_5_IRQn);
}

int main() {
  HAL_Init();
  gpio_init();
  interrupt_init();
  for (;;) {
    HAL_Delay(1000);
  }

  return 0;
}