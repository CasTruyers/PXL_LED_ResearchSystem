#include "webserver.hpp"

static const char *TAG = "webserver";

/* An HTTP GET handler */
static esp_err_t ledOffHandler(httpd_req_t *req)
{
    esp_err_t error;
    gpio_set_level(GPIO_NUM_5, 0);
    ESP_LOGI(TAG, "LED turned off");
    const char* response = (const char*) req->user_ctx;
    error = httpd_resp_send(req, response, strlen(response));
    if(error != ESP_OK)
    {
        ESP_LOGI(TAG, "Error %d while sending response", error);
    }
    else ESP_LOGI(TAG, "response sent succesfully");

    return error;
}

static esp_err_t ledOnHandler(httpd_req_t *req)
{
    esp_err_t error;
    gpio_set_level(GPIO_NUM_5, 1);
    ESP_LOGI(TAG, "LED turned on");
    const char* response = (const char*) req->user_ctx;
    error = httpd_resp_send(req, response, strlen(response));
    if(error != ESP_OK)
    {
        ESP_LOGI(TAG, "Error %d while sending response", error);
    }
    else ESP_LOGI(TAG, "response sent succesfully");

    return error;
}

static const httpd_uri_t ledOff = {
    .uri       = "/ledOff",
    .method    = HTTP_GET,
    .handler   = ledOffHandler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = const_cast<char*>("\
    <!DOCTYPE html>\
    <html>\
    <head>\
    <style>\
    .button {\
    border: none;\
    color: white;\
    padding: 15px 32px;\
    text-align: center;\
    text-decoration: none;\
    display: inline-block;\
    font-size: 16px;\
    margin: 4px 2px;\
    cursor: pointer;\
    }\
\
    .button1 {background-color: #4CAF50;} /* Green */\
    .button2 {background-color: #008CBA;} /* Blue */\
    </style>\
    </head>\
    <body>\
\
    <h1>Led-System</h1>\
\
    <button class=\"button button1\" onclick= \"window.location.href='/ledOn'\">LED ON</button>\
\
    </body>\
    </html>\
")
};

static const httpd_uri_t ledOn = {
    .uri       = "/ledOn",
    .method    = HTTP_GET,
    .handler   = ledOnHandler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = const_cast<char*>("\
    <!DOCTYPE html>\
    <html>\
    <head>\
    <style>\
    .button {\
    border: none;\
    color: white;\
    padding: 15px 32px;\
    text-align: center;\
    text-decoration: none;\
    display: inline-block;\
    font-size: 16px;\
    margin: 4px 2px;\
    cursor: pointer;\
    }\
\
    .button1 {background-color: #4CAF50;} /* Green */\
    .button2 {background-color: #008CBA;} /* Blue */\
    </style>\
    </head>\
    <body>\
\
    <h1>Led-System</h1>\
\
    <button class=\"button button2\" onclick= \"window.location.href='/ledOff'\">LED OFF</button>\
\
    </body>\
    </html>\
")
};

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &ledOff);
        httpd_register_uri_handler(server, &ledOn);
        #if CONFIG_EXAMPLE_BASIC_AUTH
        httpd_register_basic_auth(server);
        #endif
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static esp_err_t stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    return httpd_stop(server);
}

void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        if (stop_webserver(*server) == ESP_OK) {
            *server = NULL;
        } else {
            ESP_LOGE(TAG, "Failed to stop http server");
        }
    }
}

void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}