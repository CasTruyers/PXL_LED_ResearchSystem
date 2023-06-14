#include "time.hpp"
#include "esp_sntp.h"

static const char *TAG = "time";

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

void obtain_time(void)
{
    char strftime_buf[64];
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 15;

#ifdef LWIP_DHCP_GET_NTP_SRV
    sntp_servermode_dhcp(1);      // accept NTP offers from DHCP server, if any
#endif

    setenv("TZ", "EET-2EEST,M3.5.0/3,M10.5.0/4", 1); //finland time
    tzset();
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in finland is: %s", strftime_buf);
}

void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");

    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();

    ESP_LOGI(TAG, "List of configured NTP servers:");

    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i){
        if (sntp_getservername(i)){
            ESP_LOGI(TAG, "server %d: %s", i, sntp_getservername(i));
        } else {
            // we have either IPv4 or IPv6 address, let's print it
            char buff[INET6_ADDRSTRLEN];
            ip_addr_t const *ip = sntp_getserver(i);
            if (ipaddr_ntoa_r(ip, buff, INET6_ADDRSTRLEN) != NULL)
                ESP_LOGI(TAG, "server %d: %s", i, buff);
        }
    }
}

void print_current_time() {
    time_t now;
    struct tm timeinfo;
    char strftime_buf[64];

    // Get current time from RTC
    time(&now);
    localtime_r(&now, &timeinfo);
    printf("time: %d:%d\n\r", timeinfo.tm_hour, timeinfo.tm_min);
}

bool timeBigger(int setHour, int setMinute, int hour, int minute)
{
    if (hour > setHour) {
        return true; 
    } else if (hour < setHour) {
        return false;
    } else {
        if (minute > setMinute) {
            return true;
        } else if (minute < setMinute) {
            return false;
        } else {
            return true;
        }
    }
}

void check_time()
{
    time_t now;
    struct tm timeinfo;
    char strftime_buf[64];

    char on_time[6], off_time[6];
    nvs_load_on_time(on_time, sizeof(on_time));
    nvs_load_off_time(off_time, sizeof(off_time));

    int hourOn = atoi(on_time);
    int minuteOn = atoi(on_time + 3);
    int hourOff = atoi(off_time);
    int minuteOff = atoi(off_time + 3);

    time(&now);
    localtime_r(&now, &timeinfo);

    printf("on time: %d:%d, off time: %d:%d, time: %d:%d, ledOn: %d\n\r", hourOn, minuteOn, hourOff, minuteOff, timeinfo.tm_hour, timeinfo.tm_min, ledOn);

    if(ledOn)
    {
        if(timeBigger(hourOff, minuteOff, timeinfo.tm_hour, timeinfo.tm_min) && !(timeBigger(hourOn, minuteOn, timeinfo.tm_hour, timeinfo.tm_min) && timeBigger(hourOff, minuteOff, hourOn, minuteOn)))
        {
            printf("Turning lights off\n\r");
            turnOffDrivers();
            
            cJSON *object = cJSON_CreateObject();
            char action[10] = "status";
            nvs_get_JSON(object, action);
            cJSON_AddStringToObject(object, "status", "ledOff");
            send_json_to_all_clients(object);
            cJSON_Delete(object);
        }
        else printf("Lights already turned on\n\r");
    }
    else
    {
        if(timeBigger(hourOn, minuteOn, timeinfo.tm_hour, timeinfo.tm_min) && !(timeBigger(hourOff, minuteOff, timeinfo.tm_hour, timeinfo.tm_min) && timeBigger(hourOn, minuteOn, hourOff, minuteOff)))
        {
            printf("Turning lights on\n\r");
            setDrivers();

            cJSON *object = cJSON_CreateObject();
            char action[10] = "status";
            nvs_get_JSON(object, action);
            cJSON_AddStringToObject(object, "status", "ledOn");
            send_json_to_all_clients(object);
            cJSON_Delete(object);
        }
        else printf("Lights already turned off\n\r");
    }
}