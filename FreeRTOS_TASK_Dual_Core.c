#include <stdio.h>
#include "driver/gpio.h"
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"

#define Led_1 33
#define Led_2 25
#define Led_3 26

#define  STACK_SIZE   1024*2
#define DelayLed_1    1000
#define DelayLed_2    50
#define DelayLed_3    4000
const char *tag = "Main";

esp_err_t Led_Init(void);
esp_err_t creat_tasks(void);
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);

void app_main(void)
{
    Led_Init();
    creat_tasks();

    while (1)
    {
        /* code */
        vTaskDelay(pdMS_TO_TICKS(500));
        printf("Hello From Main \n");
        printf("Number of Core: %i \n",portNUM_PROCESSORS);
    }
    
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

     xTaskCreatePinnedToCore(vTask3,
                "vTask3",
                STACK_SIZE,
                &ucParameterToPass,
                1,
                &xHandle,
                tskNO_AFFINITY);                   
    return ESP_OK;
}

void vTask1(void *pvParameters)
{
    while (1)
    {
        ESP_LOGI(tag,"Led_1 Core 0");
        gpio_set_level(Led_1, 1);
        vTaskDelay(pdMS_TO_TICKS(DelayLed_1));
        gpio_set_level(Led_1, 0);
        vTaskDelay(pdMS_TO_TICKS(DelayLed_1));        
    }
}

void vTask2(void *pvParameters)
{
    while (1)
    {
        ESP_LOGW(tag,"Led_2 Core 1");
        gpio_set_level(Led_2, 1);
        vTaskDelay(pdMS_TO_TICKS(DelayLed_2));
        gpio_set_level(Led_2, 0);
        vTaskDelay(pdMS_TO_TICKS(DelayLed_2));        
    }
}

void vTask3(void *pvParameters)
{
    while (1)
    {
        ESP_LOGE(tag,"Led_3 Core ANY");
        gpio_set_level(Led_3, 1);
        vTaskDelay(pdMS_TO_TICKS(DelayLed_3));
        gpio_set_level(Led_3, 0);
        vTaskDelay(pdMS_TO_TICKS(DelayLed_3));        
    }
}