/*
 Mutex
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

#include "freertos/semphr.h"

#define STACK_SIZE 1024 * 5

const char *tag = "Main";

/* Mutex Handle ------------------------------------------------------------------*/
SemaphoreHandle_t MutexHandle = NULL;

/* Task Function ------------------------------------------------------------------*/
void Task1(void *pvParameters)
{
    /* first check the priority--------*/
    int iPriority = 0;
    iPriority = uxTaskPriorityGet(NULL);
    printf("Task1 begin: iPriority = %d \n", iPriority);

    while (1)
    {
        /* Get Mutex----------------*/
        xSemaphoreTake(MutexHandle, portMAX_DELAY);

        /* first check the priority again--------*/
        iPriority = uxTaskPriorityGet(NULL);
        printf("Task1 take: iPriority = %d \n", iPriority);

        /* loop for simulate data processing----------*/
        for (int i = 0; i < 10; i++)
        {
            iPriority = uxTaskPriorityGet(NULL);
            printf("Task1 i = %d, iPriority = %d \n", i, iPriority);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        /* Mutex Give to get by task2----------------*/
        xSemaphoreGive(MutexHandle);

        /* check the priority again if change or not --------*/
        iPriority = uxTaskPriorityGet(NULL);
        printf("Task1 give: iPriority = %d \n", iPriority);

        /* Delay for one sec.----------------*/
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void Task2(void *pvParameters)
{
    /* Delay for one sec. becouse task2 higher priority----------------*/
    vTaskDelay(pdMS_TO_TICKS(1000));
    /* first check the priority--------*/
    int iPriority = 0;
    iPriority = uxTaskPriorityGet(NULL);
    printf("Task2 begin: iPriority = %d \n", iPriority);

    while (1)
    {
        /* Get Mutex----------------*/
        xSemaphoreTake(MutexHandle, portMAX_DELAY);

        /* first check the priority again--------*/
        iPriority = uxTaskPriorityGet(NULL);
        printf("Task2 take: iPriority = %d \n", iPriority);

        /* loop for simulate data processing----------*/
        for (int i = 0; i < 10; i++)
        {
            iPriority = uxTaskPriorityGet(NULL);
            printf("Task2 i = %d, iPriority = %d \n", i, iPriority);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        /* Mutex Give to get by task2----------------*/
        xSemaphoreGive(MutexHandle);

        /* check the priority again if change or not --------*/
        iPriority = uxTaskPriorityGet(NULL);
        printf("Task2 give: iPriority = %d \n", iPriority);

        /* Delay for one sec.----------------*/
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    /* Creates a Mutex */
    MutexHandle = xSemaphoreCreateBinary();

    /* Semaphore Give first for Binary Semaphore----------------*/
        xSemaphoreGive(MutexHandle);
    if (MutexHandle != NULL)
    {
        /* Create 2 Task*/

        /*The priority of a task that holds a mutex will be raised if another task of higher
            priority attempts to obtain the same mutex.*/

        /* task1 will hold the mutex first ---------*/
        xTaskCreate(Task1, "Task1", STACK_SIZE, NULL, 1, NULL);

        /* task2 get the mutex from task1 [higher priority]---------*/
        xTaskCreate(Task2, "Task2", STACK_SIZE, NULL, 2, NULL);
    }

}
