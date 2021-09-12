#ifndef DS18B20_H
#define DS18B20_H

#include "driver/gpio.h"
#include <stdbool.h>

void testLinking();
bool initializeBus(gpio_num_t gpio_num);

#endif
