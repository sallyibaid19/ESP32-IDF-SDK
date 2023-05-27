/* FreeRTOS On ESP32: Counting Semaphore -----------------------------------*/

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

SemaphoreHandle_t semphrHandle;

/* Task Function -----------------------------------------------------------------*/
void carInTask(void *pvParameters)
{
    int emptySpace = 0;
    BaseType_t iResult;

    while (1)
    {
        emptySpace = uxSemaphoreGetCount(semphrHandle);
        printf("emptySpace = %d!\n",emptySpace);

        iResult = xSemaphoreTake(semphrHandle,0);
        if(iResult == pdPASS)
        {
            printf("One Car In!\n");
        }
        else
        {
            printf("No Space!\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
/*---------------------------------------------------------------------------------------------*/
void carOutTask(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(6000));

        xSemaphoreGive(semphrHandle);
        printf("One Car Out!\n");

    }
}
/*---------------------------------------------------------------------------------------------*/
void app_main(void)
{
    semphrHandle = xSemaphoreCreateCounting(5,5);
    xSemaphoreGive(semphrHandle);

    /* Create 2 Task with the same priority----------*/
    xTaskCreate(carInTask, "carInTask", STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(carOutTask, "carOutTask", STACK_SIZE, NULL, 1, NULL);
    
}