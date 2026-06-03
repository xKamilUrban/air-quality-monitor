#include "mqtt.h"
#include "config.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include "controller.h"  

static const char *TAG = "MQTT";
static esp_mqtt_client_handle_t client;
static bool mqtt_connected = false;

static void mqtt_event_handler(void *arg, esp_event_base_t base,
                                int32_t id, void *data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)data;
    switch (id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "CONNECTED TO BROKER");
            mqtt_connected = true;
            esp_mqtt_client_subscribe(client, MQTT_TOPIC_COMMAND, 1);
            ESP_LOGI(TAG, "SBUBSCRIBED: %s", MQTT_TOPIC_COMMAND);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "DISCONNECTED");
            mqtt_connected = false;
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "COMMAND RECEIVED: %.*s",
                     event->data_len, event->data);
            handle_command(event->data, event->data_len);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT ERROR");
            break;
        default:
            break;
    }
}

esp_err_t mqtt_manager_init(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.hostname = MQTT_BROKER_HOST,
        .broker.address.port = MQTT_BROKER_PORT,
        .broker.address.transport = MQTT_TRANSPORT_OVER_TCP,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    if (client == NULL) return ESP_FAIL;

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID,
                                   mqtt_event_handler, NULL);
    esp_err_t err = esp_mqtt_client_start(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "START ERROR: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "INIT");
    return ESP_OK;
}

void mqtt_publish_sensors(float temp, int ppm, uint8_t motor_speed)
{
    if (!mqtt_connected) return;

    char payload[64];

    snprintf(payload, sizeof(payload), "%.2f", temp);
    esp_mqtt_client_publish(client, MQTT_TOPIC_TEMP, payload, 0, 1, 0);

    snprintf(payload, sizeof(payload), "%d", ppm);
    esp_mqtt_client_publish(client, MQTT_TOPIC_GAS, payload, 0, 1, 0);

    snprintf(payload, sizeof(payload), "%d", motor_speed);
    esp_mqtt_client_publish(client, MQTT_TOPIC_MOTOR, payload, 0, 1, 0);

    ESP_LOGI(TAG, "Wysłano: temp=%.2f, ppm=%d, motor=%d", temp, ppm, motor_speed);
}

void mqtt_publish_alert(const char *message)
{
    if (!mqtt_connected) return;
    esp_mqtt_client_publish(client, MQTT_TOPIC_ALERT, message, 0, 1, 0);
    ESP_LOGW(TAG, "Alert: %s", message);
}