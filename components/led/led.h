#ifndef LED_H
#define LED_H
#include "driver/gpio.h"

esp_err_t led_init(gpio_num_t gpio_num);
esp_err_t led_set_level(gpio_num_t gpio_num, int level);
#endif