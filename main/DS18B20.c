/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
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
bool timeout;
bool acknowledge;

void testLinking(){
    printf("DS18B20.c method \"testLinking\"\n");
    fflush(stdout);
}

void hw_timer_callback(void *arg){
    timeout = true;
}

void gpio_isr_handler(void *arg){
    acknowledge = true;
}

bool initializeBus(gpio_num_t gpio_num){
    timeout = false;
    acknowledge = false;
    
    ESP_LOGI(TAG, "initializing DS18B20");
    ESP_ERROR_CHECK(gpio_set_pull_mode(gpio_num, GPIO_PULLUP_ONLY));
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    //Send reset pulse for a minimum of 480us (1ms).
    ESP_ERROR_CHECK(gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(gpio_num, 0));
    ESP_ERROR_CHECK(gpio_set_intr_type(gpio_num, GPIO_INTR_NEGEDGE));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(gpio_num, gpio_isr_handler, (void *)gpio_num));
    
    vTaskDelay(pdMS_TO_TICKS(1));
    
    //Set the bus back high, wait for input
    ESP_ERROR_CHECK(gpio_set_level(gpio_num, 1));
    ESP_ERROR_CHECK(gpio_set_direction(gpio_num, GPIO_MODE_INPUT));
    
    ESP_LOGI(TAG, "Starting timer for DS18B20 presence pulse");
    hw_timer_init(hw_timer_callback, NULL);
    hw_timer_alarm_us(200, false);
    while(!timeout && !acknowledge){}
    hw_timer_deinit();
    gpio_uninstall_isr_service();
    
    if(acknowledge != true){
        ESP_LOGE(TAG, "DS18B20 did not provide presence pulse within 60us");
        return false;
    }else{
        ESP_LOGI(TAG, "DS18B20 Provided presence pulse");
        return true;
    }
    
    
    
    
}
