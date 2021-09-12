/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "Temperature.h"
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main()
{
    xTaskCreate(temperature_task, "temp_task", 4096, NULL, 1, NULL);
    
}
