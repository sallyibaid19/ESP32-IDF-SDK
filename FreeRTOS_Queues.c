/*
FreeRTOS Queues
*/
#include <stdio.h>
#include "driver/gpio.h"
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"

#define Led_1 33
#define Led_2 25
#define Led_3 26

#define  STACK_SIZE   1024*2
#define DelayLed_1    400
#define DelayLed_2    2000

QueueHandle_t GlobalQueue = 0 ;

const char *tag = "Main";

esp_err_t Led_Init(void);
esp_err_t creat_tasks(void);
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);


void app_main(void)
{
    GlobalQueue = xQueueCreate(10,sizeof(uint32_t));
    Led_Init();
    creat_tasks();
}

esp_err_t Led_Init(void)
{
    gpio_reset_pin(Led_1);
    gpio_set_direction(Led_1, GPIO_MODE_OUTPUT);
    gpio_reset_pin(Led_2);
    gpio_set_direction(Led_2, GPIO_MODE_OUTPUT);
    gpio_reset_pin(Led_3);
    gpio_set_direction(Led_3, GPIO_MODE_OUTPUT);
    return ESP_OK;
}

esp_err_t creat_tasks(void)
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;
    xTaskCreatePinnedToCore(vTask1,
                "vTask1",
                STACK_SIZE,
                &ucParameterToPass,
                1,
                &xHandle,
                0);

    xTaskCreatePinnedToCore(vTask2,
                "vTask2",
                STACK_SIZE,
                &ucParameterToPass,
                1,
                &xHandle,
                1);                  
    return ESP_OK;
}

void vTask1(void *pvParameters)
{
    while (1)
    {
        for (size_t i =0; i<8;i++)
        {
            vTaskDelay(pdMS_TO_TICKS(DelayLed_1/2));
            gpio_set_level(Led_1, 1);
            ESP_LOGW(tag,"Sending %i to queue",i);
            if(xQueueSend(GlobalQueue,&i,pdMS_TO_TICKS(100)))
            {
                ESP_LOGE(tag,"Error Sending %i to queue",i);
            }
            vTaskDelay(pdMS_TO_TICKS(DelayLed_1/2));
            gpio_set_level(Led_1, 0);

        }      
    }
}

void vTask2(void *pvParameters)
{
    int receivedValue = 0;
    while (1)
    {
        if(!xQueueReceive(GlobalQueue,&receivedValue,pdMS_TO_TICKS(100)))
        {
            ESP_LOGE(tag,"Error Received from queue");
        }
        else
        {
             vTaskDelay(pdMS_TO_TICKS(DelayLed_2/2));
             gpio_set_level(Led_2, 1);

             ESP_LOGE(tag,"Value Received %i from queue",receivedValue);

             vTaskDelay(pdMS_TO_TICKS(DelayLed_2/2));
             gpio_set_level(Led_2, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(7000));     
    }
}

