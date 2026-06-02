#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "controller.h"
#include "wifi.h"
#include "mqtt.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "Start of the monitoring system");
    ESP_ERROR_CHECK(controller_init());
    esp_err_t wifi_err = wifi_manager_init();
    if (wifi_err != ESP_OK) {
        ESP_LOGW(TAG, "WiFi unavailable");
    }

    if (wifi_manager_is_connected()) {
        esp_err_t mqtt_err = mqtt_manager_init();
        if (mqtt_err != ESP_OK) {
            ESP_LOGW(TAG, "MQTT unavailable");
        }
    }

    xTaskCreate(controller_task, "controller_task", 4096, NULL, 5, NULL);

    ESP_LOGI(TAG, "System running");
}