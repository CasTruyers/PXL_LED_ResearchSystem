#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi.hpp"
#include "webserver.hpp"
#include "LEDdriver.hpp"
#include "time.hpp"

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_sntp.h"
#include "esp_err.h"
#include "driver/rtc_io.h"

static const char *TAG = "main";

extern "C" void app_main(void)
{  
    time_t now;
    struct tm timeinfo;
    static httpd_handle_t server = NULL;

    // initialize the GPIOs for the LED drivers
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_15);
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_5);
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_18);
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_19);

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }

    initialise_wifi();
    ESP_LOGW(TAG, "Start APSTA Mode");
	wifi_apsta(CONFIG_STA_CONNECT_TIMEOUT*1000);

    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &connect_handler, &server));

    initialize_sntp();
    time(&now);
    localtime_r(&now, &timeinfo);
    if (timeinfo.tm_year < (2023 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Getting time over NTP.");
        obtain_time();
        time(&now); // update 'now' variable with current time
    }

    check_time();
    setDrivers();

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(60000));
        check_time();
    }
}