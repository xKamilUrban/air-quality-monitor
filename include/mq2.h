#pragma once
#include "esp_err.h"
#include <stdbool.h>

esp_err_t mq2_init(void);
int       mq2_read_raw(void);
float     mq2_read_voltage(void);
int       mq2_read_ppm(void);
bool      mq2_is_safe(void);