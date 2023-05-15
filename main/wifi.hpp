#ifndef SOFTAP_HPP
#define SOFTAP_HPP

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#define CONFIG_AP_WIFI_SSID "LED_SYSTEM"
#define CONFIG_AP_WIFI_PASSWORD "LED_SYSTEM"
#define CONFIG_AP_MAX_STA_CONN 6
#define CONFIG_AP_WIFI_CHANNEL 6
#define CONFIG_STA_WIFI_SSID "TP-Link_3B8C"
#define CONFIG_STA_WIFI_PASSWORD "25820751"
#define CONFIG_STA_CONNECT_TIMEOUT 10

bool wifi_apsta(int timeout_ms);
void initialise_wifi(void);

#endif // SOFTAP_HPP