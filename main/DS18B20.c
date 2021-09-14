#include <stdio.h>
#include "DS18B20.h"
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#include "driver/hw_timer.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

static const char *TAG = "DS18B20 Driver";
int count = 0;
bool timeout;
bool present;

void hw_timer_callback(void *arg){
    timeout = true;
}

void gpio_isr_handler(void *arg){
    present = true;
    count++;
}

//This method performs a reset, and presence check on the DS18B20. 
//For timing explanation see DS18B20-Datasheet.pdf -- 1-Wire Signaling section 
bool DS18B20_InitializeBus(gpio_num_t gpio_num){
    timeout = false;
    present = false;
    
    ESP_LOGI(TAG, "initializing DS18B20");

    //Bus should be pulled high for communication with DS18B20
    ESP_ERROR_CHECK(gpio_set_pull_mode(gpio_num, GPIO_PULLUP_ONLY));
    
    //Send reset pulse for a minimum of 480us
    ESP_ERROR_CHECK(gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(hw_timer_init(hw_timer_callback, NULL));
    ESP_ERROR_CHECK(gpio_set_level(gpio_num, 0));       
    ESP_ERROR_CHECK(hw_timer_alarm_us(500, false));
    while(!timeout){}; //busy wait
    timeout = false;
    
    //Configure falling-edge interrupt to catch 'presence' signal from DS18B20
    ESP_ERROR_CHECK(gpio_set_intr_type(gpio_num, GPIO_INTR_NEGEDGE));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(gpio_num, gpio_isr_handler, (void *)gpio_num));
    
    //Set the bus back high, wait 60us max for presence signal, or timeout
    ESP_ERROR_CHECK(gpio_set_level(gpio_num, 1));
    ESP_ERROR_CHECK(gpio_set_direction(gpio_num, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(hw_timer_alarm_us(100, false));    
    
    while(!timeout && !present){
        //Wait until DS18B20 is present, or we timeout. Whichever comes first. 
    }
    
    //Release timer, isr resources
    ESP_ERROR_CHECK(hw_timer_deinit());
    ESP_ERROR_CHECK(gpio_isr_handler_remove(gpio_num));
    gpio_uninstall_isr_service();
    ESP_ERROR_CHECK(gpio_set_intr_type(gpio_num, GPIO_INTR_DISABLE));
    
    return present; 
}

void DS18B20_WriteBit(gpio_num_t gpio_num, bool bit){
    timeout = false;
    ESP_ERROR_CHECK(gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(gpio_num, 0));
    ESP_ERROR_CHECK(hw_timer_init(hw_timer_callback, NULL));
    ESP_ERROR_CHECK(gpio_set_level(gpio_num, bit));
    if(bit){
        ESP_ERROR_CHECK(gpio_set_direction(gpio_num, 1));
    }
    ESP_ERROR_CHECK(hw_timer_alarm_us(60, false));
    while(!timeout){}; //busy wait
    timeout = false;
    //Release Bus
    ESP_ERROR_CHECK(gpio_set_direction(gpio_num, 1));
    ESP_ERROR_CHECK(gpio_set_direction(gpio_num, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(hw_timer_alarm_us(11, false));
    while(!timeout){};
    ESP_ERROR_CHECK(hw_timer_deinit());
}

void DS18B20_WriteByte(gpio_num_t gpio_num, uint8_t byte){
    ESP_LOGI(TAG, "Writing byte 0x%x", byte);
    for(int i = 0; i < 8; i++){
        DS18B20_WriteBit(gpio_num, (byte & 1<<i) > 0);
    }
}

uint8_t DS18B20_ReadBit(gpio_num_t gpio_num){
    timeout = false;
    present = false;
    ESP_ERROR_CHECK(gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(gpio_num, 0));
    ESP_ERROR_CHECK(hw_timer_init(hw_timer_callback, NULL));
    ESP_ERROR_CHECK(gpio_set_intr_type(gpio_num, GPIO_INTR_NEGEDGE));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(gpio_num, gpio_isr_handler, (void *)gpio_num));
    ESP_ERROR_CHECK(gpio_set_level(gpio_num, 1));
    ESP_ERROR_CHECK(gpio_set_direction(gpio_num, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(hw_timer_alarm_us(11, false));
    while(!timeout){}; //busy wait
    timeout = false;
    ESP_ERROR_CHECK(gpio_isr_handler_remove(gpio_num));
    gpio_uninstall_isr_service();
    ESP_ERROR_CHECK(gpio_set_intr_type(gpio_num, GPIO_INTR_DISABLE));
    ESP_ERROR_CHECK(hw_timer_alarm_us(50, false));
    while(!timeout){};
    ESP_ERROR_CHECK(hw_timer_deinit());
    return (uint8_t)(!present);

}

uint8_t DS18B20_ReadByte(gpio_num_t gpio_num){
    uint8_t response = 0;
    for(int i = 0; i < 8; i++){
        response = response | (DS18B20_ReadBit(gpio_num)<<i);
    }
    //ESP_LOGI(TAG, "Read byte 0x%2x", response);
    return response;
}

