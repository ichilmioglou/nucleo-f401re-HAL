#include "SEGGER_RTT.h"
#include "lv_conf.h"
#include "lvgl.h"
#include "st7789.h"
#include "stm32f4xx_hal.h"
#include <string.h>

#define DISP_BUF_SIZE 240 * 40
/**
 * lvgl-demo using HAL Library and ST7789 display driver
 *
 * SPI1_SCK     = PA5
 * SPI1_MOSI    = PA7
 * ST7789_RES   = PB6
 * ST7789_DC    = PC7
 * ST7789_CS    = PA9
 * ST7789_BLK   = PA8
 *
 */

SPI_HandleTypeDef spi1;

void gpio_init() {
  // enable clocks
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  // init SPI pins
  GPIO_InitTypeDef gpio = {0};
  gpio.Alternate = GPIO_AF5_SPI1;
  gpio.Mode = GPIO_MODE_AF_PP;
  gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio.Pull = GPIO_NOPULL;
  gpio.Pin = GPIO_PIN_5 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOA, &gpio);

  memset(&gpio, 0, sizeof(gpio));

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
  // init output pins
  gpio.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &gpio);
  gpio.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOB, &gpio);
  gpio.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &gpio);
}

void spi1_init() {
  __HAL_RCC_SPI1_CLK_ENABLE();
  spi1.Instance = SPI1;
  // spi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; // 16MHz/16 = 1MHz
  spi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  spi1.Init.DataSize = SPI_DATASIZE_8BIT;
  spi1.Init.Direction = SPI_DIRECTION_2LINES;
  spi1.Init.Mode = SPI_MODE_MASTER;
  spi1.Init.NSS = SPI_NSS_SOFT;
  spi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  spi1.Init.CLKPhase = SPI_POLARITY_HIGH;
  spi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  HAL_SPI_Init(&spi1);
}

void display_flush(lv_disp_drv_t *drv, const lv_area_t *area,
                   lv_color_t *color_map) {
  ST7789_SetAddressWindow(area->x1, area->y1, area->x2, area->y2);
  uint32_t size = lv_area_get_width(area) * lv_area_get_height(area);

  ST7789_WriteData((uint8_t *)color_map, size * 2);

  lv_disp_t *disp = _lv_refr_get_disp_refreshing();
  lv_disp_flush_ready(disp->driver);
}

static void anim_x_cb(void *var, int32_t v) {
  lv_obj_set_x((lv_obj_t *)var, v);
}

static void anim_size_cb(void *var, int32_t v) {
  lv_obj_set_size((lv_obj_t *)var, v, v);
}

int main() {
  HAL_Init();
  gpio_init();
  spi1_init();
  ST7789_Init();
  lv_init();

  lv_color_t *buf = (lv_color_t *)malloc(DISP_BUF_SIZE * sizeof(lv_color_t));
  static lv_disp_draw_buf_t draw_buf;
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, DISP_BUF_SIZE);

  // Initialize display
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = display_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = 240;
  disp_drv.ver_res = 240;

  lv_disp_drv_register(&disp_drv);

  lv_obj_t *obj = lv_obj_create(lv_scr_act());
  lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE), 0);
  lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

  lv_obj_align(obj, LV_ALIGN_LEFT_MID, 10, 0);

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj);
  lv_anim_set_values(&a, 10, 50);
  lv_anim_set_time(&a, 1000);
  lv_anim_set_playback_delay(&a, 100);
  lv_anim_set_playback_time(&a, 300);
  lv_anim_set_repeat_delay(&a, 500);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

  lv_anim_set_exec_cb(&a, anim_size_cb);
  lv_anim_start(&a);
  lv_anim_set_exec_cb(&a, anim_x_cb);
  lv_anim_set_values(&a, 10, 150);
  lv_anim_start(&a);
  for (;;) {
    HAL_Delay(10);
    lv_task_handler();
    lv_tick_inc(10);
  }

  return 0;
}