#pragma once
#include "esp_err.h"

esp_err_t controller_init(void);
void      controller_task(void *pvParameters);