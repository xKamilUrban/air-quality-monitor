#include "lps22df.h"
#include "config.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static i2c_master_bus_handle_t bus_handle;
static i2c_master_dev_handle_t dev_handle;

static esp_err_t i2c_master_init(void){
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA,
        .scl_io_num = I2C_MASTER_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .flags.enable_internal_pullup = true,
    };

    esp_err_t esp_err = i2c_new_master_bus(&bus_config, &bus_handle);
    if (esp_err != ESP_OK) return esp_err;

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = LPS22DF_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    esp_err = i2c_master_bus_add_device (bus_handle, &dev_config, &dev_handle);
    return esp_err; 
}

esp_err_t lps22df_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    return i2c_master_transmit_receive(dev_handle, &reg_addr, 1, data, len, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
}

static esp_err_t lps22df_write_byte(uint8_t reg_addr, uint8_t data)
{
    uint8_t write_buf[2] = {reg_addr, data};
    return i2c_master_transmit(dev_handle, write_buf, sizeof(write_buf), pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
}

esp_err_t lps22df_init(void){
    uint8_t data[1];

    esp_err_t err = i2c_master_init();
    if (err != ESP_OK) {
        ESP_LOGE("LPS", "I2C init ERROR: %s", esp_err_to_name(err));
        return err;
    }

    err = lps22df_read(LPS22DF_WHO_AM_I, data, 1);
    if (err != ESP_OK) {
        ESP_LOGE("LPS", "WHO_AM_I ERROR: %s", esp_err_to_name(err));
        return err;
    }

    if (data[0] != 0xB4) {
        ESP_LOGE("LPS", "WHO_AM_I ERROR: 0x%02X", data[0]);
        return ESP_ERR_NOT_FOUND;
    }
    ESP_LOGI("LPS", "WHO_AM_I OK: 0x%02X", data[0]);

    return lps22df_write_byte(LPS22DF_CTRL_REG1, LPS22DF_CTRL_REG1_VAL);
}

float lps22df_read_temperature(void)
{
    uint8_t data[2];
    if (lps22df_read(LPS22DF_TEMP_OUT_L, data, 2) != ESP_OK) return -999.0f;
    int16_t raw = (int16_t)((data[1] << 8) | data[0]);
    return (float)raw / 100.0f;
}