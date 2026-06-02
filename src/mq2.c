#include "mq2.h"
#include "config.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

static const char *TAG = "MQ2";
static adc_oneshot_unit_handle_t adc_handle;

esp_err_t mq2_init(void)
{
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    esp_err_t err = adc_oneshot_new_unit(&init_cfg, &adc_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ADC init ERROR: %s", esp_err_to_name(err));
        return err;
    }

    adc_oneshot_chan_cfg_t chan_cfg = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten    = ADC_ATTEN_DB_11,
    };
    err = adc_oneshot_config_channel(adc_handle, MQ2_ADC_PIN, &chan_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ADC channel ERROR: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "INIT");
    return ESP_OK;
}

int mq2_read_raw(void)
{
    int raw = 0;
    adc_oneshot_read(adc_handle, MQ2_ADC_PIN, &raw);
    return raw;
}

float mq2_read_voltage(void)
{
    return (float)mq2_read_raw() * 3.3f / 4095.0f;
}

int mq2_read_ppm(void)
{
    return (int)((float)mq2_read_raw() / 4095.0f * 10000.0f);
}

bool mq2_is_safe(void)
{
    return mq2_read_voltage() < (MQ2_VOLTAGE_LIMIT_MV / 1000.0f);
}