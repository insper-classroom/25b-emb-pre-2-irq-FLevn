#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;
volatile int btn_flag = 0;
volatile int btn_state = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) {  // fall edge
        if (gpio == BTN_PIN_R) {
            btn_flag = 1;
            btn_state = 1;
        } else if (gpio == BTN_PIN_G) {
            btn_flag = 1;
            btn_state = 0;
        }
    } else {
        btn_flag = 0;
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    // callback led r (first)
    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);

    // callback led g (nao usar _with_callback)
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    while (true) {
        if (btn_flag) {
            if (btn_state) {
                printf("fall red\n");
                btn_flag = 0;
            } else {
                printf("fall green\n");
                btn_flag = 0;
            }
        }
    }
}