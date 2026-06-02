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
#define WIFI_SSID               "SSID"
#define WIFI_PASSWORD           "PASSWORD"
#define WIFI_MAX_RETRY          5

// MQTT
#define MQTT_BROKER_URI         "mqtt://192.168.1.100"
#define MQTT_TOPIC_TEMP         "home/sensor/temperature"
#define MQTT_TOPIC_GAS          "home/sensor/gas"
#define MQTT_TOPIC_MOTOR        "home/sensor/motor"
#define MQTT_TOPIC_ALERT        "home/alert"
#define MQTT_PUBLISH_INTERVAL   5000