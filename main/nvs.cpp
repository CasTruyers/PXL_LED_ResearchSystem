#include "nvs.hpp"

void nvs_save_time(cJSON *timeJson) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    ESP_ERROR_CHECK(err);

    cJSON *onTime = cJSON_GetObjectItem(timeJson, "onTime");
    cJSON *offTime = cJSON_GetObjectItem(timeJson, "offTime");
    printf("saving onTime:  %s, offTime: %s to the NVS\n\r", onTime->valuestring, offTime->valuestring);

    err = nvs_set_str(handle, "on_time", onTime->valuestring);
    ESP_ERROR_CHECK(err);
    err = nvs_set_str(handle, "off_time", offTime->valuestring);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(handle);
    ESP_ERROR_CHECK(err);

    nvs_close(handle);
}

void nvs_load_time(cJSON *timeJSON) {
    char on_time[6], off_time[6];
    size_t size = sizeof(on_time);
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
    ESP_ERROR_CHECK(err);
    printf("get str ontime\n\r");
    err = nvs_get_str(handle, "on_time", on_time, &size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) ESP_ERROR_CHECK(err);
    printf("get str offtime\n\r");
    err = nvs_get_str(handle, "off_time", off_time, &size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) ESP_ERROR_CHECK(err);
    printf("add ontime to object\n\r");
    cJSON_AddStringToObject(timeJSON, "onTime", on_time);
    printf("add offtime to object\n\r");
    cJSON_AddStringToObject(timeJSON, "offTime", off_time);

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

    uint8_t firstDriverDCint = (uint8_t)strtoul(firstDriverDC->valuestring, NULL, 10);
    uint8_t secondDriverDCint = (uint8_t)strtoul(secondDriverDC->valuestring, NULL, 10);
    uint8_t thirdDriverDCint = (uint8_t)strtoul(thirdDriverDC->valuestring, NULL, 10);
    uint8_t fourthDriverDCint = (uint8_t)strtoul(fourthDriverDC->valuestring, NULL, 10);
    printf("setting NVS with: %d, %d, %d, %d", firstDriverDCint, secondDriverDCint, thirdDriverDCint, fourthDriverDCint);

    err = nvs_set_u8(handle, "firstDriverDC", firstDriverDCint);
    ESP_ERROR_CHECK(err);
    err = nvs_set_u8(handle, "secondDriverDC", secondDriverDCint);
    ESP_ERROR_CHECK(err);
    err = nvs_set_u8(handle, "thirdDriverDC", thirdDriverDCint);
    ESP_ERROR_CHECK(err);
    err = nvs_set_u8(handle, "fourthDriverDC", fourthDriverDCint);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(handle);
    ESP_ERROR_CHECK(err);

    nvs_close(handle);
}

void nvs_load_drivers(cJSON *driversJson)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    ESP_ERROR_CHECK(err);

    cJSON *first_driver_dc_json = cJSON_CreateObject();
    cJSON *second_driver_dc_json = cJSON_CreateObject();
    cJSON *third_driver_dc_json = cJSON_CreateObject();
    cJSON *fourth_driver_dc_json = cJSON_CreateObject();

    uint8_t first_driver_dc = 0;
    uint8_t second_driver_dc = 0;
    uint8_t third_driver_dc = 0;
    uint8_t fourth_driver_dc = 0;
    size_t max_length = sizeof(first_driver_dc);

    err = nvs_get_u8(handle, "firstDriverDC", &first_driver_dc);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("\n\rsetting default first_driver_dc value in NVS");
        err = nvs_set_u8(handle, "firstDriverDC", 0);
        if (err != ESP_OK) printf("ERROR");}

    err = nvs_get_u8(handle, "secondDriverDC", &second_driver_dc);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("\n\rsetting default second_driver_dc value in NVS\n\r");
        err = nvs_set_u8(handle, "secondDriverDC", 0);
        if (err != ESP_OK) printf("ERROR");}

    err = nvs_get_u8(handle, "thirdDriverDC", &third_driver_dc);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("\n\rsetting default third_driver_dc value in NVS\n\r");
        err = nvs_set_u8(handle, "thirdDriverDC", 0);
        if (err != ESP_OK) printf("ERROR");}

    err = nvs_get_u8(handle, "fourthDriverDC", &fourth_driver_dc);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("\n\rsetting default fourth_driver_dc value in NVS\n\r");
        err = nvs_set_u8(handle, "fourthDriverDC", 0);
        if (err != ESP_OK) printf("ERROR\n\r");}

    printf("uint8t DC: %d, %d, %d, %d",first_driver_dc, second_driver_dc, third_driver_dc, fourth_driver_dc);

    cJSON_AddNumberToObject(first_driver_dc_json, "dutyCycle", (const double)first_driver_dc);
    cJSON_AddNumberToObject(second_driver_dc_json, "dutyCycle", (const double)second_driver_dc);
    cJSON_AddNumberToObject(third_driver_dc_json, "dutyCycle", (const double)third_driver_dc);
    cJSON_AddNumberToObject(fourth_driver_dc_json, "dutyCycle", (const double)fourth_driver_dc);
    cJSON_AddItemToObject(driversJson, "firstDriver", first_driver_dc_json);
    cJSON_AddItemToObject(driversJson, "secondDriver", second_driver_dc_json);
    cJSON_AddItemToObject(driversJson, "thirdDriver", third_driver_dc_json);
    cJSON_AddItemToObject(driversJson, "fourthDriver", fourth_driver_dc_json);

    nvs_close(handle);
}

void nvs_get_JSON(cJSON *object, char* action)
{
    cJSON *drivers = cJSON_CreateObject();
    cJSON *time = cJSON_CreateObject();
    cJSON_AddStringToObject(object, "action", action);
    nvs_load_drivers(drivers);
    cJSON_AddItemToObject(object, "drivers", drivers);
    nvs_load_time(time);
    cJSON_AddItemToObject(object, "time", time);
    if(ledOn) cJSON_AddStringToObject(object, "status", "ledOn");
    else cJSON_AddStringToObject(object, "status", "ledOff");
    
}