#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "LEDdriver.hpp"

#define NUM_LEDS 4

void fadeAll(LEDDriver* leds) {
    for (float dutyCycle = 0; dutyCycle < 100; dutyCycle++) {
        for (int j = 0; j < NUM_LEDS; j++) {
            leds[j].setDuty(1023 * (dutyCycle/100)); //1023 for 10BIT
        }
        vTaskDelay(pdMS_TO_TICKS(25));
    }
    for (float dutyCycle = 100; dutyCycle > 0; dutyCycle--) {
        for (int j = 0; j < NUM_LEDS; j++) {
            leds[j].setDuty(1023 * (dutyCycle/100)); //1023 for 10BIT
        }
        vTaskDelay(pdMS_TO_TICKS(25));
    }
}

extern "C" void app_main(void)
{
    // initialize the GPIOs for the LED drivers
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_12);
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_13);
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_15);
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_2);

    // create the LED drivers
    LEDDriver leds[NUM_LEDS] = {
        LEDDriver(GPIO_NUM_12, LEDC_TIMER_0, LEDC_CHANNEL_0),
        LEDDriver(GPIO_NUM_13, LEDC_TIMER_0, LEDC_CHANNEL_1),
        LEDDriver(GPIO_NUM_15, LEDC_TIMER_0, LEDC_CHANNEL_2),
        LEDDriver(GPIO_NUM_2, LEDC_TIMER_0, LEDC_CHANNEL_3)
    };

    while(true)
    {
        fadeAll(leds);     
    }
}