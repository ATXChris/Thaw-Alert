#include "Temperature.h"
#include <stdio.h>
#include "DS18B20.h"

#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

static const char *TAG = "Temperature Task";

void temperature_task(void *pvParameters){

    //Pin "D2" is GPIO_NUM_4
    if(DS18B20_InitializeBus(GPIO_NUM_4)){
        ESP_LOGI(TAG, "DS18B20 Initialized Successfully");
        DS18B20_WriteByte(GPIO_NUM_4, 0xCC);
        DS18B20_WriteByte(GPIO_NUM_4, 0xBE);
        DS18B20_ReadByte(GPIO_NUM_4);
        DS18B20_ReadByte(GPIO_NUM_4);
        DS18B20_ReadByte(GPIO_NUM_4);
        DS18B20_ReadByte(GPIO_NUM_4);
        DS18B20_ReadByte(GPIO_NUM_4);
        DS18B20_ReadByte(GPIO_NUM_4);
        DS18B20_ReadByte(GPIO_NUM_4);
        DS18B20_ReadByte(GPIO_NUM_4);
        DS18B20_ReadByte(GPIO_NUM_4);
    }else{
        ESP_LOGE(TAG, "Sensor failed to intialize");
    }
    

    while(1){
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

