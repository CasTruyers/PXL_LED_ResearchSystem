#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "esp_tls_crypto.h"
#include "driver/gpio.h"
#include "esp_spiffs.h"
#include "spi_flash_mmap.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <sys/stat.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include "nvs.hpp"
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include "protocol_examples_common.h"
#include <esp_http_server.h>
#include "cJSON.h"

#include "LEDdriver.hpp"

#define EXAMPLE_HTTP_QUERY_KEY_MAX_LEN  (64)
#define LED_PIN GPIO_NUM_5

extern LEDDriver LEDDrivers[NUM_LEDS];

void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

#endif //WEBSERVER_HPP