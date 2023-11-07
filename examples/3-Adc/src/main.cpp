#include "SEGGER_RTT.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
/**
 * ADC demo using HAL
 *
 * use pin PA6
 */

ADC_HandleTypeDef adc1 = {0};
void gpio_init() {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef gpio = {0};
  gpio.Mode = GPIO_MODE_ANALOG;
  gpio.Pull = GPIO_PULLDOWN;
  gpio.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOA, &gpio);
}

void adc_init() {
  __HAL_RCC_ADC1_CLK_ENABLE();
  // Configure the global features of the ADC (Clock, Resolution, Data Alignment
  // and number of conversion)
  adc1.Instance = ADC1;
  adc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  adc1.Init.Resolution = ADC_RESOLUTION12b;
  adc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  adc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  adc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&adc1) != HAL_OK) {
    SEGGER_RTT_printf(0, "Error ADC_Init\n");
  }

  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&adc1, &sConfig)) {
    SEGGER_RTT_printf(0, "Error ADC_ConfigChannel\n");
  }
}

int main() {
  HAL_Init();
  gpio_init();
  adc_init();

  for (;;) {
    HAL_StatusTypeDef ret = HAL_OK;
    HAL_ADC_Start(&adc1);
    ret = HAL_ADC_PollForConversion(&adc1, 1000);
    if (ret) {
      SEGGER_RTT_printf(0, "ret = %02X\n", ret);
    }

    uint32_t adc_raw = HAL_ADC_GetValue(&adc1);

    SEGGER_RTT_printf(0, "adc_raw=%lu\n", adc_raw);
    HAL_Delay(1000);
  }

  return 0;
}