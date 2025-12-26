#ifndef BUTTON_H
#define BUTTON_H
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#define DEBOUNCE_MS 50
#define BTN_NONE -1
#define BTN_PRESS 1
#define BTN_RELEASE 0

typedef struct button_t {
    gpio_num_t pin;
    int last_read;
    TickType_t last_tick;
    int stable_state;
} button_t;

esp_err_t btn_init(button_t* btn, gpio_num_t pin);
int btn_read(button_t* btn);
#endif