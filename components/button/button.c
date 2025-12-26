#include "button.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



esp_err_t btn_init(button_t* btn, gpio_num_t pin)
{
    gpio_config_t config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = 1ULL << pin,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE
    };
    btn->pin = pin;
    btn->last_read = gpio_get_level(pin);
    btn->last_tick = xTaskGetTickCount();
    btn->stable_state = btn->last_read;

    return gpio_config(&config);
}


int btn_read(button_t* btn)
{
    int current = gpio_get_level(btn->pin);
    TickType_t now = xTaskGetTickCount();

    if (current != btn->last_read)
    {
        btn->last_read = current;
        btn->last_tick = now;
        return BTN_NONE;
    }
    if ((now - btn->last_tick) * portTICK_PERIOD_MS >= DEBOUNCE_MS)
    {
        if (btn->stable_state != current) {
            btn->stable_state = current;
            return (btn->stable_state == 1) ? BTN_PRESS : BTN_RELEASE;
        }
    }
    return BTN_NONE;
}
