#ifndef DS18B20_H
#define DS18B20_H

#include "driver/gpio.h"
#include <stdbool.h>

bool DS18B20_InitializeBus(gpio_num_t gpio_num);
void DS18B20_WriteByte(gpio_num_t gpio_num, uint8_t byte);
uint8_t DS18B20_ReadByte(gpio_num_t gpio_num);

#endif
