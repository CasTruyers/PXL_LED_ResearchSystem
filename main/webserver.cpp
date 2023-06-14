#include "webserver.hpp"

static const char *TAG = "webserver";
int led_state = 0;
httpd_handle_t server = NULL;

#define INDEX_HTML_PATH "/spiffs/index.html"
char index_html[20000];

// Read spiff and place index.html in buffer index_html
static void initi_web_page_buffer(void)
{
    printf("In function \"initi_web_page_buffer\": Read spiff and place index.html in buffer index_html\n\r");
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};

    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));

    memset((void *)index_html, 0, sizeof(index_html));
    struct stat st;
    if (stat(INDEX_HTML_PATH, &st))
    {
        ESP_LOGE(TAG, "index.html not found");
        return;
    }

    FILE *fp = fopen(INDEX_HTML_PATH, "r");
    if (fread(index_html, st.st_size, 1, fp) == 0)
    {
        ESP_LOGE(TAG, "fread failed");
    }
    fclose(fp);
}

// used to send a web page to the client in response to an HTTP request.
esp_err_t get_req_handler(httpd_req_t *req)
{
    return httpd_resp_send(req, index_html, HTTPD_RESP_USE_STRLEN);
}

void send_json_to_all_clients(cJSON *object)
{

    char *json_str = cJSON_Print(object);
    printf("json str: %s\n\r", json_str);
    httpd_ws_frame_t ws_pkt;
    ws_pkt.payload = (uint8_t *)json_str;
    ws_pkt.len = strlen(json_str);
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    static size_t max_clients = CONFIG_LWIP_MAX_LISTENING_TCP;
    size_t fds = max_clients;
    int client_fds[max_clients];

    esp_err_t ret = httpd_get_client_list(server, &fds, client_fds);
    if (ret != ESP_OK)
        return;

    for (int i = 0; i < fds; i++)
    {
        int client_info = httpd_ws_get_fd_info(server, client_fds[i]);
        if (client_info == HTTPD_WS_CLIENT_WEBSOCKET)
        {
            httpd_ws_send_frame_async(server, client_fds[i], &ws_pkt);
            printf("Sended to client\n\r");
        }
    }
    printf("JSON sended to all clients\n\r");
}

static esp_err_t handle_ws_req(httpd_req_t *req)
{
    if (req->method == HTTP_GET)
    {
        ESP_LOGI(TAG, "Handshake done, the new connection was opened");
        cJSON *object = cJSON_CreateObject();
        printf("getJsonFunc going in\n\r");
        char action[10] = "updateAll";
        nvs_get_JSON(object, action);
        printf("out of getJsonFunc\n\r");
        send_json_to_all_clients(object);
        cJSON_Delete(object);
        return ESP_OK;
    }

    httpd_ws_frame_t ws_pkt;
    uint8_t *buf = NULL;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed to get frame len with %d", ret);
        return ret;
    }

    if (ws_pkt.len)
    {
        buf = static_cast<uint8_t *>(calloc(1, ws_pkt.len + 1));
        if (buf == NULL)
        {
            ESP_LOGE(TAG, "Failed to calloc memory for buf");
            return ESP_ERR_NO_MEM;
        }
        ws_pkt.payload = buf; // set to same point as buf
        ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
        if (ret != ESP_OK)
        {
            ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
            free(buf);
            return ret;
        }
        ESP_LOGI(TAG, "Got packet with message: %s", ws_pkt.payload);
    }

    ESP_LOGI(TAG, "frame len is %d", ws_pkt.len);

    // Parse JSON string into a cJSON object
    cJSON *object = cJSON_ParseWithLength(reinterpret_cast<const char *>(ws_pkt.payload), ws_pkt.len);

    if (object == nullptr)
    {
        printf("Failed to parse JSON string\n\r");
        return 1;
    }

    cJSON *action = cJSON_GetObjectItemCaseSensitive(object, "action");
    if (cJSON_IsString(action))
    {
        char *actionValue = action->valuestring;
        printf("Action: %s\n\r", actionValue);
        if (strcmp(actionValue, "dutyCycle") == 0)
        {
            send_json_to_all_clients(object);
            cJSON *driversJson = cJSON_GetObjectItem(object, "drivers");
            nvs_save_drivers(driversJson);
            setDrivers();
        }
        else if (strcmp(actionValue, "time") == 0)
        {
            cJSON *timeJson = cJSON_GetObjectItem(object, "time");
            nvs_save_time(timeJson);

            char on_time[6], off_time[6];
            nvs_load_on_time(on_time, sizeof(on_time));
            nvs_load_off_time(off_time, sizeof(off_time));
            printf("Loaded onTime: %s, offTime: %s from the NVS\n\r", on_time, off_time);
        }
        else
            printf("action does not exist\n\r");
    }
    else
        printf("action not found\n\r");

    cJSON_Delete(object);

    return ESP_OK;
}

httpd_handle_t setup_websocket_server(void)
{
    printf("In setup_websocket_server\r\n");
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_uri_t uri_get = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = get_req_handler,
        .user_ctx = NULL};

    httpd_uri_t ws = {
        .uri = "/ws",
        .method = HTTP_GET,
        .handler = handle_ws_req,
        .user_ctx = NULL,
        .is_websocket = true};

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &ws);
    }

    return server;
}

static esp_err_t stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    return httpd_stop(server);
}

void disconnect_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;
    if (*server)
    {
        ESP_LOGI(TAG, "Stopping webserver");
        if (stop_webserver(*server) == ESP_OK)
        {
            *server = NULL;
        }
        else
        {
            ESP_LOGE(TAG, "Failed to stop http server");
        }
    }
}

void connect_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;
    if (*server == NULL)
    {
        ESP_LOGI(TAG, "Starting webserver");
        printf("In connect handler\r\n");
        initi_web_page_buffer();
        *server = setup_websocket_server();
    }
}