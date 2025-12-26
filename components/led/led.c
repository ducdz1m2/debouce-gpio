#include "led.h"

esp_err_t led_init(gpio_num_t gpio_num)
{
    gpio_config_t config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << gpio_num,
    };

    return gpio_config(&config);
}
esp_err_t led_set_level(gpio_num_t gpio_num, int level)
{
    return gpio_set_level(gpio_num, level);
}
