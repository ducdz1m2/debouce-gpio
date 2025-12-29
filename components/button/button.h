#ifndef BUTTON_H
#define BUTTON_H
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define DEBOUNCE_MS 50
#define BTN_DOUBLE_MS 300
#define BTN_LONG_MS 1000

#define BTN_NONE -1
#define BTN_PRESS 1
#define BTN_RELEASE 0
#define BTN_DOUBLE 2
#define BTN_LONG_PRES 3

typedef enum {
    BTN_STATE_IDLE,     
    BTN_STATE_PRESSED,  
} btn_state_t;

typedef enum {
    BTN_EVT_NONE,
    BTN_EVT_PRESS,
    BTN_EVT_DOUBLE,
    BTN_EVT_LONG,
} btn_event_t;


typedef struct {
    gpio_num_t pin;

    int last_raw;
    TickType_t last_debounce;

    btn_state_t state;

    TickType_t press_tick;
    TickType_t last_click_tick;

    uint8_t click_count;
    uint8_t long_sent;
} button_t;


esp_err_t btn_init(button_t *btn, gpio_num_t pin);
btn_event_t btn_read(button_t *btn);
#endif