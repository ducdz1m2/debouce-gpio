#include "button.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_err_t btn_init(button_t *btn, gpio_num_t pin)
{
    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << pin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    ESP_ERROR_CHECK(gpio_config(&cfg));

    int level = gpio_get_level(pin);

    btn->pin = pin;
    btn->last_raw = level;
    btn->last_debounce = xTaskGetTickCount();

    btn->state = BTN_STATE_IDLE;
    btn->press_tick = 0;
    btn->last_click_tick = 0;
    btn->click_count = 0;
    btn->long_sent = 0;

    return ESP_OK;
}

btn_event_t btn_read(button_t *btn)
{
    int raw = gpio_get_level(btn->pin);
    TickType_t now = xTaskGetTickCount();

    /* debounce raw */
    if (raw != btn->last_raw)
    {
        btn->last_raw = raw;
        btn->last_debounce = now;
        return BTN_EVT_NONE;
    }

    if ((now - btn->last_debounce) < pdMS_TO_TICKS(DEBOUNCE_MS))
        return BTN_EVT_NONE;

    switch (btn->state)
    {

    case BTN_STATE_IDLE:
        if (raw == 0)
        {
            btn->state = BTN_STATE_PRESSED;
            btn->press_tick = now;
            btn->long_sent = 0;
        }
        break;

    case BTN_STATE_PRESSED:

        if (!btn->long_sent &&
            (now - btn->press_tick) >= pdMS_TO_TICKS(BTN_LONG_MS))
        {

            btn->long_sent = 1;
            btn->click_count = 0;
            return BTN_EVT_LONG;
        }

        if (raw == 1)
        {
            btn->state = BTN_STATE_IDLE;

            if (btn->long_sent)
                break;

            btn->click_count++;
            btn->last_click_tick = now;

            if (btn->click_count == 2)
            {
                btn->click_count = 0;
                return BTN_EVT_DOUBLE;
            }
        }
        break;
    }

    if (btn->click_count == 1 &&
        (now - btn->last_click_tick) > pdMS_TO_TICKS(BTN_DOUBLE_MS))
    {

        btn->click_count = 0;
        return BTN_EVT_PRESS;
    }

    return BTN_EVT_NONE;
}
