#ifndef LED_DRIVER_HPP
#define LED_DRIVER_HPP

#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "stdio.h"
#include "cJSON.h"
#include <nvs_flash.h>
#include "nvs.hpp"

#define NUM_LEDS 4

extern bool ledOn;

class LEDDriver {
public:
    LEDDriver(gpio_num_t gpio, ledc_timer_t timer, ledc_channel_t channel);
    void setDuty(uint8_t duty);
    void fade();
private:
    gpio_num_t m_gpio;
    ledc_timer_t m_timer;
    ledc_channel_t m_channel;
    void initPWM();
};

void fadeAll(LEDDriver* leds);
void setDrivers();
void turnOffDrivers();

#endif // LED_DRIVER_HPP