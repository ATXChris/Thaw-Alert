#ifndef DS18B20_H
#define DS18B20_H

#include "driver/gpio.h"
#include <stdbool.h>

bool DS18B20_InitializeBus(gpio_num_t gpio_num);

#endif
