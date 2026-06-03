#pragma once
#include "driver/gpio.h"
#include "driver/i2c_master.h"

// I2C
#define I2C_MASTER_SCL          GPIO_NUM_22
#define I2C_MASTER_SDA          GPIO_NUM_21
#define I2C_MASTER_NUM          I2C_NUM_0
#define I2C_MASTER_FREQ_HZ      400000
#define I2C_MASTER_TIMEOUT_MS   1000

// MQ2
#define MQ2_ADC_PIN             ADC_CHANNEL_6   // GPIO34
#define MQ2_VOLTAGE_LIMIT_MV    3000

// Motor PWM
#define MOTOR_PWM_PIN           GPIO_NUM_25
#define MOTOR_PWM_FREQ          5000
#define MOTOR_PWM_RESOLUTION    LEDC_TIMER_8_BIT  // 0-255

// Progi sterowania
#define GAS_THRESHOLD_LOW       1000
#define GAS_THRESHOLD_HIGH      3000
#define TEMP_THRESHOLD_HIGH     30.0f

// WiFi
#define WIFI_SSID               "realme 14 5G n4p4"
#define WIFI_PASSWORD           "AsdAsd123"
#define WIFI_MAX_RETRY          5

// MQTT
#define MQTT_BROKER_HOST        "broker.emqx.io"
#define MQTT_BROKER_PORT        1883
#define MQTT_TOPIC_TEMP         "/sensor/temperature"
#define MQTT_TOPIC_GAS          "/sensor/gas"
#define MQTT_TOPIC_MOTOR        "/sensor/motor"
#define MQTT_TOPIC_ALERT        "/alert"
#define MQTT_TOPIC_COMMAND      "/command"
#define MQTT_PUBLISH_INTERVAL   5000