#include <stdio.h>
#include <config.h>
#include "button.h"
#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

button_t btn1;


void app_main(void)
{
    btn_init(&btn1, BUTTON_PIN);
    led_init(LED_PIN);

    int led_state = 0;

    while (1)
    {
        int btn_evt = btn_read(&btn1);
        if (btn_evt == BTN_PRESS)
        {
            led_state = !led_state;
            led_set_level(LED_PIN, led_state);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
