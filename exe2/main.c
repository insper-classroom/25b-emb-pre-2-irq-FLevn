#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN = 4;
volatile int btn_flag = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events & GPIO_IRQ_EDGE_FALL) { // fall edge
    btn_flag = 1;
  } else if (events & GPIO_IRQ_EDGE_RISE) { // rise edge
    btn_flag = 0;
  }
}

int main() {
  stdio_init_all();
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);
  gpio_set_irq_enabled(BTN_PIN_R, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
  gpio_set_irq_callback(&btn_callback);
  while (true) {
    if (btn_flag) {
      gpio_put(LED_PIN, 1);
    } else {
      gpio_put(LED_PIN, 0);
    }
  }
}
