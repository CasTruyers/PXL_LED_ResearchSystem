#include "LEDdriver.hpp"

LEDDriver LEDDrivers[NUM_LEDS] = {
    LEDDriver(GPIO_NUM_15, LEDC_TIMER_0, LEDC_CHANNEL_0),
    LEDDriver(GPIO_NUM_5, LEDC_TIMER_0, LEDC_CHANNEL_1),
    LEDDriver(GPIO_NUM_18, LEDC_TIMER_0, LEDC_CHANNEL_2),
    LEDDriver(GPIO_NUM_19, LEDC_TIMER_0, LEDC_CHANNEL_3)
};

bool ledOn = true;

// CLASS FUNCTIONS

// Initialize the LED driver
LEDDriver::LEDDriver(gpio_num_t gpio, ledc_timer_t timer, ledc_channel_t channel)
    : m_gpio(gpio), m_timer(timer), m_channel(channel)
{
    initPWM();
}

// Initialize PWM for the LED driver
void LEDDriver::initPWM()
{
    // Configure timer
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = m_timer,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_config);

    // Configure channel
    ledc_channel_config_t channel_config = {
        .gpio_num = m_gpio,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = m_channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = m_timer,
        .duty = 0,
        .hpoint = 0,
        .flags = {
            .output_invert = 0, // Set to 1 if you want to invert the output signal
        }
    };
    ledc_channel_config(&channel_config);

    printf("LEDdriver gpio %d Initialized\n", m_gpio);

    // Set duty to 0%
    setDuty(0);
}

// Set the duty cycle for the LED driver
void LEDDriver::setDuty(uint8_t dutyCycle)
{
    uint32_t dutyVal = dutyCycle * 2;
    printf("dutycycle: %d\n\r", dutyCycle);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel, dutyVal);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel);
    if(dutyCycle > 0) ledOn = true;
}

void LEDDriver::fade() {
    for (int i = 0; i <= 100; i++) {
        setDuty(i);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    for (int i = 100; i >= 0; i--) {
        setDuty(i);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// NON-CLASS FUNCTIONS

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

void setDrivers()
{
    printf("Reading NVS for drivers DC\n\r");
    cJSON *driversJson = cJSON_CreateObject();
    nvs_load_drivers(driversJson);
    printf("%s",cJSON_Print(driversJson));

    cJSON *first_driver_dc_obj = cJSON_GetObjectItem(cJSON_GetObjectItem(driversJson, "firstDriver"), "dutyCycle");
    cJSON *second_driver_dc_obj = cJSON_GetObjectItem(cJSON_GetObjectItem(driversJson, "secondDriver"), "dutyCycle");
    cJSON *third_driver_dc_obj = cJSON_GetObjectItem(cJSON_GetObjectItem(driversJson, "thirdDriver"), "dutyCycle");
    cJSON *fourth_driver_dc_obj = cJSON_GetObjectItem(cJSON_GetObjectItem(driversJson, "fourthDriver"), "dutyCycle");
    const uint8_t first_driver_dc = first_driver_dc_obj->valueint;
    const uint8_t second_driver_dc = second_driver_dc_obj->valueint;
    const uint8_t third_driver_dc = third_driver_dc_obj->valueint;
    const uint8_t fourth_driver_dc = fourth_driver_dc_obj->valueint;

    printf("Setting drivers: %d, %d, %d, %d\n\r", first_driver_dc_obj->valueint, second_driver_dc_obj->valueint, third_driver_dc_obj->valueint, fourth_driver_dc_obj->valueint);
    LEDDrivers[0].setDuty(first_driver_dc);
    LEDDrivers[1].setDuty(second_driver_dc);
    LEDDrivers[2].setDuty(third_driver_dc);
    LEDDrivers[3].setDuty(fourth_driver_dc);
    cJSON_Delete(driversJson);
}

void turnOffDrivers()
{
    LEDDrivers[0].setDuty(0);
    LEDDrivers[1].setDuty(0);
    LEDDrivers[2].setDuty(0);
    LEDDrivers[3].setDuty(0);
    ledOn = false;
}