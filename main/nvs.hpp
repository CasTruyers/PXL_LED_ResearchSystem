#ifndef NVS_HPP
#define NVS_HPP

#include <nvs_flash.h>
#include "nvs.h"
#include "cJSON.h"

void nvs_save_time(cJSON *timeJson);
void nvs_load_on_time(char* on_time, size_t max_length);
void nvs_load_off_time(char* off_time, size_t max_length);
void nvs_load_time(cJSON *setTime, size_t max_length);
void nvs_save_drivers(cJSON *driverJson);
void nvs_load_drivers(cJSON *drivers);

#endif //NVS_HPP