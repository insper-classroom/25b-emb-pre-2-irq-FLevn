#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN = 28;

volatile int btn_flag = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (gpio == BTN_PIN) {
        if (events & GPIO_IRQ_EDGE_FALL) {
            btn_flag = 1;
        }
        else if (events & GPIO_IRQ_EDGE_RISE) {
            btn_flag = 0;
        }
    }
}

int main()
{
    stdio_init_all();

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    gpio_set_irq_enabled_with_callback(
        BTN_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);

    while (true) {
        if (btn_flag) {
            sleep_ms(800);
            if (!btn_flag) {
                printf("Aperto curto!\n");
            } else {
                while (btn_flag) {
                    sleep_ms(100);
                }
                printf("Aperto longo!\n");
            }
        }
    }
}
