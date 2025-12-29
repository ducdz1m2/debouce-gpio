#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "button.h"
#include "led.h"
#include "config.h"

/* ================== STATIC OBJECT ================== */
static button_t btn1;
static led_t led1;
static QueueHandle_t btn_evt_queue;

/* ================== BUTTON TASK ================== */
static void button_task(void *arg)
{
    (void)arg;

    while (1)
    {
        btn_event_t evt = btn_read(&btn1);

        if (evt != BTN_EVT_NONE)
        {
            xQueueSend(btn_evt_queue, &evt, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/* ================== LED TASK ================== */
static void led_task(void *arg)
{
    (void)arg;
    btn_event_t evt;

    while (1)
    {
        if (xQueueReceive(btn_evt_queue, &evt, portMAX_DELAY))
        {

            switch (evt)
            {

            case BTN_EVT_PRESS:
                led_toggle(&led1);
                break;

            case BTN_EVT_DOUBLE:
                led_toggle(&led1);
                vTaskDelay(pdMS_TO_TICKS(100));
                led_toggle(&led1);
                break;

            case BTN_EVT_LONG:
                for (int i = 0; i < 4; i++)
                {
                    led_toggle(&led1);
                    vTaskDelay(pdMS_TO_TICKS(100));
                }
                break;

            default:
                break;
            }
        }
    }
}

/* ================== APP MAIN ================== */
void app_main(void)
{
    btn_evt_queue = xQueueCreate(4, sizeof(btn_event_t));
    configASSERT(btn_evt_queue);

    btn_init(&btn1, BUTTON_PIN);
    led_init(&led1, LED_PIN);

    xTaskCreate(
        button_task,
        "button_task",
        2048,
        NULL,
        5,
        NULL);

    xTaskCreate(
        led_task,
        "led_task",
        2048,
        NULL,
        5,
        NULL);
}
