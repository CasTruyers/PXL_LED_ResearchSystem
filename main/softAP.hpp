#ifndef SOFTAP_HPP
#define SOFTAP_HPP

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define EXAMPLE_ESP_WIFI_SSID      "LED_SYSTEM"
#define EXAMPLE_ESP_WIFI_PASS      "LED_SYSTEM"
#define EXAMPLE_ESP_WIFI_CHANNEL   0
#define EXAMPLE_MAX_STA_CONN       5

void wifi_init_softap(void);

#endif // SOFTAP_HPP