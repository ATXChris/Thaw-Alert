#include "Temperature.h"
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main()
{
    xTaskCreate(temperature_task, "temp_task", 4096, NULL, 1, NULL);
}
