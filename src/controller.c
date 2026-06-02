#include "controller.h"
#include "config.h"
#include "lps22df.h"
#include "mq2.h"
#include "motor.h"
#include "mqtt.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "CONTROLLER";

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
    if (temp >= TEMP_THRESHOLD_HIGH && ppm < GAS_THRESHOLD_LOW) {
        return 80;
    }

    if (ppm < GAS_THRESHOLD_LOW) return 0;
    if (ppm >= GAS_THRESHOLD_HIGH) return 255;

    float ratio = (float)(ppm - GAS_THRESHOLD_LOW) /
                  (float)(GAS_THRESHOLD_HIGH - GAS_THRESHOLD_LOW);
    return (uint8_t)(ratio * 255.0f);
}

void controller_task(void *pvParameters)
{
    TickType_t last_publish = 0;

    while (1) {
        float temp = lps22df_read_temperature();
        int   ppm  = mq2_read_ppm();

        ESP_LOGI(TAG, "Temp: %.2f°C | Gaz: %d ppm",
                 temp, ppm);

        uint8_t speed = calculate_motor_speed(ppm, temp);
        motor_set_speed(speed);

        TickType_t now = xTaskGetTickCount();
        if ((now - last_publish) >= pdMS_TO_TICKS(MQTT_PUBLISH_INTERVAL)) {
            mqtt_publish_sensors(temp, ppm, speed);
            last_publish = now;

            if (ppm >= GAS_THRESHOLD_HIGH) {
                mqtt_publish_alert("WARNING! HIGH GAS CONCENTRATION!");
            }
            if (temp >= TEMP_THRESHOLD_HIGH) {
                mqtt_publish_alert("WARNING! HIGH TEMPERATURE!");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}