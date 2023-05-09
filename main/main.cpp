#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "softAP.hpp"
#include "webserver.hpp"
#include "LEDdriver.hpp"

extern "C" void app_main(void)
{   
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
    
    // Read/initialize the drivers NVS and set drivers dutycycle accordingly
    cJSON *driversJson = cJSON_CreateObject();
    setDrivers(driversJson, 1);

    wifi_init_softap();
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}