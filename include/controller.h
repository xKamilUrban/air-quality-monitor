#pragma once
#include "esp_err.h"
#include <stdbool.h>

extern float g_temp_threshold;
extern int   g_gas_threshold;
extern bool  g_motor_manual;

esp_err_t controller_init(void);
void      controller_task(void *pvParameters);
void      handle_command(const char *data, int len);