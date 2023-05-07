#include "nvs.hpp"

void nvs_save_time(cJSON *timeJson) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    ESP_ERROR_CHECK(err);

    cJSON *onTime = cJSON_GetObjectItem(timeJson, "onTime");
    cJSON *offTime = cJSON_GetObjectItem(timeJson, "offTime");
    printf("saving onTime:  %s and offTime: %s to the NVS\n\r", onTime->valuestring, offTime->valuestring);

    err = nvs_set_str(handle, "on_time", onTime->valuestring);
    ESP_ERROR_CHECK(err);
    err = nvs_set_str(handle, "off_time", offTime->valuestring);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(handle);
    ESP_ERROR_CHECK(err);

    nvs_close(handle);
}

void nvs_load_time(cJSON *setTime, size_t max_length) {
    char on_time[6], off_time[6];
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
    ESP_ERROR_CHECK(err);

    err = nvs_get_str(handle, "on_time", on_time, &max_length);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_ERROR_CHECK(err);
    }

    err = nvs_get_str(handle, "off_time", off_time, &max_length);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_ERROR_CHECK(err);
    }

    cJSON_AddStringToObject(setTime, "onTime", on_time);
    cJSON_AddStringToObject(setTime, "offTime", off_time);

    nvs_close(handle);
}

void nvs_load_on_time(char* on_time, size_t max_length) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
    ESP_ERROR_CHECK(err);

    err = nvs_get_str(handle, "on_time", on_time, &max_length);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_ERROR_CHECK(err);
    }

    nvs_close(handle);
}

void nvs_load_off_time(char* off_time, size_t max_length) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
    ESP_ERROR_CHECK(err);

    err = nvs_get_str(handle, "off_time", off_time, &max_length);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_ERROR_CHECK(err);
    }

    nvs_close(handle);
}

void nvs_save_drivers(cJSON *driversJson)
{
    printf("In nvs_save_drivers()\n\r");
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    ESP_ERROR_CHECK(err);

    cJSON *firstDriverDC = cJSON_GetObjectItem(cJSON_GetObjectItem(driversJson, "firstDriver"), "dutyCycle");
    cJSON *secondDriverDC = cJSON_GetObjectItem(cJSON_GetObjectItem(driversJson, "secondDriver"), "dutyCycle");
    cJSON *thirdDriverDC = cJSON_GetObjectItem(cJSON_GetObjectItem(driversJson, "thirdDriver"), "dutyCycle");
    cJSON *fourthDriverDC = cJSON_GetObjectItem(cJSON_GetObjectItem(driversJson, "fourthDriver"), "dutyCycle");
    //printf("%s\t%s\t%s\t%s", firstDriverDC->valuestring, secondDriverDC->valuestring, thirdDriverDC->valuestring, fourthDriverDC->valuestring);

    err = nvs_set_str(handle, "firstDriverDC", firstDriverDC->valuestring);
    ESP_ERROR_CHECK(err);
    err = nvs_set_str(handle, "secondDriverDC", secondDriverDC->valuestring);
    ESP_ERROR_CHECK(err);
    err = nvs_set_str(handle, "thirdDriverDC", thirdDriverDC->valuestring);
    ESP_ERROR_CHECK(err);
    err = nvs_set_str(handle, "fourthDriverDC", fourthDriverDC->valuestring);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(handle);
    ESP_ERROR_CHECK(err);

    nvs_close(handle);
}

// void nvs_load_dutyCycle(cJSON *root, size_t max_length)
// {

// }
