#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "softAP.hpp"
#include "webserver.hpp"
#include "LEDdriver.hpp"

// create the LED drivers
LEDDriver LEDDrivers[NUM_LEDS] = {
    LEDDriver(GPIO_NUM_12, LEDC_TIMER_0, LEDC_CHANNEL_0),
    LEDDriver(GPIO_NUM_13, LEDC_TIMER_0, LEDC_CHANNEL_1),
    LEDDriver(GPIO_NUM_15, LEDC_TIMER_0, LEDC_CHANNEL_2),
    LEDDriver(GPIO_NUM_2, LEDC_TIMER_0, LEDC_CHANNEL_3)
};

void fadeAll(LEDDriver* leds) {
    for (float dutyCycle = 0; dutyCycle < 100; dutyCycle++) {
        for (int j = 0; j < NUM_LEDS; j++) {
            leds[j].setDuty(dutyCycle);
        }
        vTaskDelay(pdMS_TO_TICKS(25));
    }
    for (float dutyCycle = 100; dutyCycle > 0; dutyCycle--) {
        for (int j = 0; j < NUM_LEDS; j++) {
            leds[j].setDuty(dutyCycle);
        }
        vTaskDelay(pdMS_TO_TICKS(25));
    }
}

extern "C" void app_main(void)
{   
    static httpd_handle_t server = NULL;
    // initialize the GPIOs for the LED drivers
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_12);
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_13);
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_15);
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_2);

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    wifi_init_softap();
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}