#include "motor.h"
#include "config.h"
#include "driver/ledc.h"
#include "esp_log.h"

static const char *TAG = "MOTOR";
static uint8_t current_speed = 0;

esp_err_t motor_init(void)
{
    ledc_timer_config_t timer = {
        .speed_mode      = LEDC_LOW_SPEED_MODE,
        .timer_num       = LEDC_TIMER_0,
        .duty_resolution = MOTOR_PWM_RESOLUTION,
        .freq_hz         = MOTOR_PWM_FREQ,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    esp_err_t err = ledc_timer_config(&timer);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Timer ERROR: %s", esp_err_to_name(err));
        return err;
    }

    ledc_channel_config_t channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = LEDC_CHANNEL_0,
        .timer_sel  = LEDC_TIMER_0,
        .intr_type  = LEDC_INTR_DISABLE,
        .gpio_num   = MOTOR_PWM_PIN,
        .duty       = 0,
        .hpoint     = 0,
    };
    err = ledc_channel_config(&channel);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Channel ERROR: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "INIT");
    return ESP_OK;
}

void motor_set_speed(uint8_t speed)
{
    current_speed = speed;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, speed);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    ESP_LOGI(TAG, "SPEED: %d/255", speed);
}

void motor_stop(void)
{
    motor_set_speed(0);
}

uint8_t motor_get_speed(void)
{
    return current_speed;
}