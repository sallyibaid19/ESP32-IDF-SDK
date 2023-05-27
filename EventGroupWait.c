/* FreeRTOS On ESP32: Event Group Wait -----------------------------------*/

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

#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#define STACK_SIZE 1024 * 5
const char *tag = "Main";

/* Create Event Group Handle ------------------------------------------------------*/
EventGroupHandle_t EventGroupHandle = NULL;

/* bits that want to be set ------------------------------------------------------*/
#define BIT_0 (1 << 0) // bit0
#define BIT_4 (1 << 4) // bit4

/* Task Function -----------------------------------------------------------------*/
void Task1(void *pvParameters)
{
    while (1)
    {
        /* first check the Event Group--------*/
        printf("------------------------------\n");
        printf("Task1 begin to check!\n");
        xEventGroupWaitBits(
            EventGroupHandle,
            BIT_0 | BIT_4,
            pdTRUE,
            pdTRUE,//pdFALSE,
            portMAX_DELAY);

        printf("In Task1, BIT_0 or BIT_4 is set!\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
/*---------------------------------------------------------------------------------------------*/
void Task2(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1)
    {
        printf("------------------------------\n");
        printf("Task2 begin to set BIT_0!\n");

        xEventGroupSetBits(
            EventGroupHandle,
            BIT_0);

        vTaskDelay(pdMS_TO_TICKS(5000));
        
        printf("Task2 begin to set BIT_4!\n");
        xEventGroupSetBits(
            EventGroupHandle,
            BIT_4);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
/*---------------------------------------------------------------------------------------------*/
void app_main(void)
{
    /* Creates an one Event Group define-----------*/
    EventGroupHandle = xEventGroupCreate();

    if (EventGroupHandle != NULL)
    {
        /* Create 2 Task with the same priority----------*/
        xTaskCreate(Task1, "Task1", STACK_SIZE, NULL, 1, NULL);
        xTaskCreate(Task2, "Task2", STACK_SIZE, NULL, 1, NULL);
    }
}