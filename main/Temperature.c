#include "Temperature.h"
#include <stdio.h>
#include <math.h>
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

#define SCRATCHPAD_SIZE 9
#define TO_FAHRENHEIT(Celsius) (Celsius * 1.8	+32)

static const char *TAG = "Temperature Task";

void printWord(uint16_t byte){
    char byteChars[17];
    for(int i = 0; i < 16; i++){
        if((byte & (1<<(15-i))) > 0){
            byteChars[i] = '1';
        }else{
            byteChars[i] = '0';
        }
    }
    byteChars[16] = '\0';
    ESP_LOGD(TAG, "Word: %s", byteChars);

}

void temperatureConversion(gpio_num_t gpio_num){
    DS18B20_InitializeBus(gpio_num);
    DS18B20_WriteByte(gpio_num, 0xCC);
    DS18B20_WriteByte(gpio_num, 0x44);
}

float readTemperature(gpio_num_t gpio_num){
    DS18B20_InitializeBus(gpio_num);
    DS18B20_WriteByte(gpio_num, 0xCC);
    DS18B20_WriteByte(gpio_num, 0xBE);
    uint8_t scratchPad[SCRATCHPAD_SIZE];
    for(int i = 0; i < SCRATCHPAD_SIZE; i++){
        scratchPad[i] = DS18B20_ReadByte(gpio_num);
    }
    uint16_t tempBits = scratchPad[1];
    tempBits = tempBits << 8;
    tempBits = tempBits | scratchPad[0];
    printWord(tempBits);
    float sign = 1;
    if((tempBits & 0xF000) > 0){
        sign = -1;
        tempBits -= 1;
        tempBits = ~tempBits;
    }
    float degreesCelsius = 0;
    
    for(int i = 0; i < 11; i++){
        if((tempBits & (1<<i)) > 0){
            degreesCelsius += pow(2, i-4);
        }
    }
    degreesCelsius *= sign;
    
    /*
    int32_t intDegreesCelsius;
    intDegreesCelsius = degreesCelsius * 10000;
    ESP_LOGI(TAG, "Temperature C: %d.%04u", intDegreesCelsius/10000, abs(intDegreesCelsius) % 10000);
    
    float degreesFahrenheit = degreesCelsius * 1.8 + 32;
    int32_t intDegreesFahrenheit = degreesFahrenheit * 10000;
    ESP_LOGI(TAG, "Temperature C: %d.%04u", intDegreesFahrenheit/10000, abs(intDegreesFahrenheit) % 10000);
    */
    
    return degreesCelsius;
    
}

void temperature_task(void *pvParameters){
    //Pin "D2" is GPIO_NUM_4
    while(1){
	temperatureConversion(GPIO_NUM_4);
	vTaskDelay(pdMS_TO_TICKS(1000));
	readTemperature(GPIO_NUM_4);
    }
}

