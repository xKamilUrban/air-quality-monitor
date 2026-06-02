#pragma once
#include "esp_err.h"

esp_err_t motor_init(void);
void      motor_set_speed(uint8_t speed);   // 0-255
void      motor_stop(void);
uint8_t   motor_get_speed(void);