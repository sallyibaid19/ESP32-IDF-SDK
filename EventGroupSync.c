/* FreeRTOS On ESP32: Event Group Sync -----------------------------------*/

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
EventGroupHandle_t xEventBits ;

/* bits that want to be set ------------------------------------------------------*/
#define TASK_0_BIT (1 << 0) // bit0
#define TASK_1_BIT (1 << 1) // bit1
#define TASK_2_BIT (1 << 2) // bit2
#define ALL_SYNC_BITS  (TASK_0_BIT | TASK_1_BIT | TASK_2_BIT)
/* Task Function -----------------------------------------------------------------*/
void Task0(void *pvParameters)
{
    while (1)
    {
        /* first check the Event Group--------*/
        printf("------------------------------\n");
        printf("Task0 begin !\n");

        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("------------------------------\n");
        printf("Task0 Set Bit0 !\n");   

        xEventGroupSync(
            xEventBits,
            TASK_0_BIT,     /*The Bit to Set*/
            ALL_SYNC_BITS,  /*The Bit to wait for */
            portMAX_DELAY); /*Timeout value.*/

        printf("Task0 Sync!\n");
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
/*---------------------------------------------------------------------------------------------*/
void Task1(void *pvParameters)
{
    while (1)
    {
        /* first check the Event Group--------*/
        printf("------------------------------\n");
        printf("Task1 begin !\n");

        vTaskDelay(pdMS_TO_TICKS(3000));

        printf("------------------------------\n");
        printf("Task1 Set Bit1 !\n");   

        xEventGroupSync(
            xEventBits,
            TASK_1_BIT,     /*The Bit to Set*/
            ALL_SYNC_BITS,  /*The Bit to wait for */
            portMAX_DELAY); /*Timeout value.*/

        printf("Task1 Sync!\n");
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
/*---------------------------------------------------------------------------------------------*/
void Task2(void *pvParameters)
{
    while (1)
    {
        /* first check the Event Group--------*/
        printf("------------------------------\n");
        printf("Task2 begin !\n");

        vTaskDelay(pdMS_TO_TICKS(5000));

        printf("------------------------------\n");
        printf("Task2 Set Bit2 !\n");   

        xEventGroupSync(
            xEventBits,
            TASK_2_BIT,     /*The Bit to Set*/
            ALL_SYNC_BITS,  /*The Bit to wait for */
            portMAX_DELAY); /*Timeout value.*/

        printf("Task2 Sync!\n");
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
/*---------------------------------------------------------------------------------------------*/
void app_main(void)
{
    /* Attempt to Create the Event Group -----------*/
    xEventBits = xEventGroupCreate();

    /* Wait the Event Group Created Successfully ? -----------*/
    if (xEventBits == NULL)
    {
        /* The Event Group was not Created Because there was insufficient FreeRTOS Heap Available.----------*/
        printf("Event Group Create Fail !\n");

    }
    else
    {
        /* The Event Group was Created-----------*/
        vTaskSuspendAll();

        xTaskCreate(Task0, "Task0", STACK_SIZE, NULL, 1, NULL);
        xTaskCreate(Task1, "Task1", STACK_SIZE, NULL, 1, NULL);
        xTaskCreate(Task2, "Task2", STACK_SIZE, NULL, 1, NULL);

        xTaskResumeAll();
    }
}