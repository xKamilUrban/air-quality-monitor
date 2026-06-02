#pragma once

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"

#define LPS22DF_ADDR            0x5D
#define LPS22DF_WHO_AM_I        0x0F
#define LPS22DF_CTRL_REG1       0x10
#define LPS22DF_STATUS          0x27
#define LPS22DF_PRESS_OUT_XL    0x28
#define LPS22DF_TEMP_OUT_L      0x2B
#define LPS22DF_CTRL_REG1_VAL   0x08

void lps22df_data_task(void *pvParameters);
esp_err_t lps22df_init(void);
void lps22df_data_task(void *pvParameters);
float lps22df_read_temperature(void);