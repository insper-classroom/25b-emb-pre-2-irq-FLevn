#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;
const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int led_state_r = 0;
volatile int led_state_g = 0;
volatile int btn_flag = 0;

void red_btn_callback(uint gpio, uint32_t events) {
  if (events & GPIO_IRQ_EDGE_FALL) {
    btn_flag = 1;
    printf("Red button pressed\n");
    if (led_state_r == 0) {
      led_state_r = 1;
    } else {
      led_state_r = 0;
    }
  }
}

void green_btn_callback(uint gpio, uint32_t events) {
  if (events & GPIO_IRQ_EDGE_RISE) {
    btn_flag = 1;
    printf("Green button pressed\n");
    if (led_state_g == 0) {
      led_state_g = 1;
    } else {
      led_state_g = 0;
    }
  }
}

int main() {
  stdio_init_all();

  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);
  gpio_put(LED_PIN_R, 0);

  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);
  gpio_put(LED_PIN_G, 0);

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  gpio_set_irq_enabled_with_callback(
    BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &red_btn_callback
  );
  gpio_set_irq_enabled_with_callback(
    BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true, &green_btn_callback
  );

  while (true) {
    if (btn_flag) {
      gpio_put(LED_PIN_R, led_state_r);
      gpio_put(LED_PIN_G, led_state_g);
      btn_flag = 0;
    }
    sleep_ms(100);
  }
}
