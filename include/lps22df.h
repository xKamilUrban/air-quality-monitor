#pragma once

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"

#define I2C_MASTER_SCL          GPIO_NUM_22
#define I2C_MASTER_SDA          GPIO_NUM_21
#define I2C_MASTER_NUM          I2C_NUM_0
#define I2C_MASTER_FREQ_HZ      400000
#define I2C_MASTER_TIMEOUT_MS   1000

#define LPS22DF_ADDR            0x5C
#define LPS22DF_WHO_AM_I        0x0F
#define LPS22DF_CTRL_REG1       0x10
#define LPS22DF_CTRL_REG2       0x11
#define LPS22DF_STATUS          0x27
#define LPS22DF_PRESS_OUT_XL    0x28
#define LPS22DF_PRESS_OUT_L     0x29
#define LPS22DF_PRESS_OUT_H     0x2A
#define LPS22DF_TEMP_OUT_L      0x2B
#define LPS22DF_TEMP_OUT_H      0x2C
#define LPS22DF_CTRL_REG1_VAL   0x08

void lps22df_data_task(void *pvParameters);
esp_err_t lps22df_init(void);