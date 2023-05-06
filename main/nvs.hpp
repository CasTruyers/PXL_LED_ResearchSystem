#ifndef NVS_HPP
#define NVS_HPP

#include <nvs_flash.h>
#include "nvs.h"

void save_on_time(char* on_time_str);
void save_off_time(char* off_time_str);
void load_on_time(char* on_time_str, size_t max_length);
void load_off_time(char* off_time_str, size_t max_length);

#endif //NVS_HPP