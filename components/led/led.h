#ifndef LED_H
#define LED_H
#include "driver/gpio.h"

typedef struct led_t {
    int led_state;
    gpio_num_t pin;
} led_t;


esp_err_t led_init(led_t* led, gpio_num_t gpio_num);
esp_err_t led_toggle(led_t* led);
#endif