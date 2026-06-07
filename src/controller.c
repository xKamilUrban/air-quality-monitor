#include "controller.h"
#include "config.h"
#include "lps22df.h"
#include "mq2.h"
#include "motor.h"
#include "mqtt.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h> 
#include <stdlib.h> 

static const char *TAG = "CONTROLLER";

float g_temp_threshold = TEMP_THRESHOLD_HIGH;
int   g_gas_threshold  = GAS_THRESHOLD_HIGH;
bool  g_motor_manual   = false;

esp_err_t controller_init(void)
{
    esp_err_t err;

    err = lps22df_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "LPS22DF init ERROR");
        return err;
    }

    err = mq2_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "MQ2 init ERROR");
        return err;
    }

    err = motor_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Motor init ERROR");
        return err;
    }

    return ESP_OK;
}

static uint8_t calculate_motor_speed(int ppm, float temp)
{
    if (temp >= g_temp_threshold && ppm < g_gas_threshold) {
        return 80;
    }

    if (ppm < g_gas_threshold) return 0;
    if (ppm >= GAS_THRESHOLD_HIGH) return 255;

    float ratio = (float)(ppm - g_gas_threshold) /
                  (float)(GAS_THRESHOLD_HIGH - g_gas_threshold);
    return (uint8_t)(ratio * 255.0f);
}

void handle_command(const char *data, int len)
{
    ESP_LOGI(TAG, "COMMAND: %.*s", len, data);

    if (strstr(data, "motor_off")) {
        g_motor_manual = true;
        motor_stop();
        ESP_LOGI(TAG, "Motor off (manual)");

    } else if (strstr(data, "motor_on")) {
        g_motor_manual = true;
        motor_set_speed(128);
        ESP_LOGI(TAG, "Motor on (manual)");

    } else if (strstr(data, "motor_auto")) {
        g_motor_manual = false;
        ESP_LOGI(TAG, "Motor auto");

    } else if (strstr(data, "set_speed")) {
        char *val = strstr(data, "\"value\":");
        if (val) {
            uint8_t speed = (uint8_t)atoi(val + 8);
            g_motor_manual = true;
            motor_set_speed(speed);
            ESP_LOGI(TAG, "Set speed: %d", speed);
        }

    } else if (strstr(data, "set_temp_threshold")) {
        char *val = strstr(data, "\"value\":");
        if (val) {
            g_temp_threshold = atof(val + 8);
            ESP_LOGI(TAG, "Temp threshold: %.1f°C", g_temp_threshold);
        }

    } else if (strstr(data, "set_gas_threshold")) {
        char *val = strstr(data, "\"value\":");
        if (val) {
            g_gas_threshold = atoi(val + 8);
            ESP_LOGI(TAG, "Gas threshold: %d PPM", g_gas_threshold);
        }
    }
}

void controller_task(void *pvParameters)
{
    TickType_t last_publish = 0;

    while (1) {
        float temp = lps22df_read_temperature();
        int   ppm  = mq2_read_ppm();

        int   raw  = mq2_read_raw();
        float volt = mq2_read_voltage();

        ESP_LOGI(TAG, "Temp: %.2f°C | RAW: %d | Voltage: %.2fV | PPM: %d",
                 temp, raw, volt, ppm);

        if (!g_motor_manual) {
            uint8_t speed = calculate_motor_speed(ppm, temp);
            motor_set_speed(speed);
        }

        TickType_t now = xTaskGetTickCount();
        if ((now - last_publish) >= pdMS_TO_TICKS(MQTT_PUBLISH_INTERVAL)) {
            mqtt_publish_sensors(temp, ppm, motor_get_speed());
            last_publish = now;

            if (ppm >= g_temp_threshold) {
                mqtt_publish_alert("WARNING! HIGH GAS CONCENTRATION!");
            }
            if (temp >= g_temp_threshold) {
                mqtt_publish_alert("WARNING! HIGH TEMPERATURE!");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}