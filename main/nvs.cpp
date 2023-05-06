#include "nvs.hpp"

void save_on_time(char* on_time_str) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    ESP_ERROR_CHECK(err);

    err = nvs_set_str(handle, "on_time", on_time_str);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(handle);
    ESP_ERROR_CHECK(err);

    nvs_close(handle);
}

void save_off_time(char* off_time_str) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    ESP_ERROR_CHECK(err);

    err = nvs_set_str(handle, "off_time", off_time_str);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(handle);
    ESP_ERROR_CHECK(err);

    nvs_close(handle);
}

void load_on_time(char* on_time_str, size_t max_length) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
    ESP_ERROR_CHECK(err);

    err = nvs_get_str(handle, "on_time", on_time_str, &max_length);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_ERROR_CHECK(err);
    }

    nvs_close(handle);
}

void load_off_time(char* off_time_str, size_t max_length) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
    ESP_ERROR_CHECK(err);

    err = nvs_get_str(handle, "off_time", off_time_str, &max_length);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_ERROR_CHECK(err);
    }

    nvs_close(handle);
}
