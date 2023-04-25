#include "LEDdriver.hpp"

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
    dutyCycle = static_cast<uint8_t>(255 * (dutyCycle/100.0));
    printf("dutycycle: %d\n\r", dutyCycle);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel, dutyCycle);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel);
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