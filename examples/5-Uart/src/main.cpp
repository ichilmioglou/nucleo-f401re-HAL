#include "SEGGER_RTT.h"
#include "stm32f4xx_hal.h"
/**
 * UART Example
 *
 * PA9:RX     PA10: TX
 *
 */

UART_HandleTypeDef uart1 = {0};

void uart1_init() {
  // initialize gpio
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef gpio = {0};
  gpio.Pin = GPIO_PIN_9 | GPIO_PIN_10;
  gpio.Mode = GPIO_MODE_AF_PP;
  gpio.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &gpio);
  // initialize usart1
  __HAL_RCC_USART1_CLK_ENABLE();

  uart1.Instance = USART1;
  uart1.Init.BaudRate = 115200;
  uart1.Init.Mode = UART_MODE_TX_RX;
  uart1.Init.StopBits = UART_STOPBITS_1;
  uart1.Init.WordLength = UART_WORDLENGTH_8B;
  if (HAL_UART_Init(&uart1) != HAL_OK) {
    SEGGER_RTT_printf(0, "Error initalizing uart1\n");
  }
}
int main() {
  HAL_Init();
  uart1_init();
  const char *text = "Hello world\n";
  for (;;) {
    HAL_UART_Transmit(&uart1, (uint8_t *)text, 12, 1000);
    HAL_Delay(1000);
  }

  return 0;
}