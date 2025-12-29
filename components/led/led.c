#include "led.h"

esp_err_t led_init(led_t *led, gpio_num_t pin)
{
    gpio_config_t config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << pin,
    };
    led->pin = pin;
    led->led_state = gpio_get_level(pin);
    return gpio_config(&config);
}


esp_err_t led_toggle(led_t *led)
{
    led->led_state = !led->led_state;
    return gpio_set_level(led->pin, led->led_state);
}
