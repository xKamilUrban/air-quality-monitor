#pragma once
#include "esp_err.h"

esp_err_t mqtt_manager_init(void);
void      mqtt_publish_sensors(float temp, int ppm, uint8_t motor_speed);
void      mqtt_publish_alert(const char *message);