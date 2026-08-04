#include "wifi.h"

static const char *TAG="WIFI.CPP";
static const char settings_file[]="/littlefs/wifi.json";

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    Wifi *wifi=(Wifi *)arg;
    switch (event_id)   {
        case WIFI_EVENT_STA_START:
            wifi->flags|=WIFI_FLG_STA_READY;
            if (wifi->flags&WIFI_FLG_VALID) {
                esp_wifi_connect();
            }
        break;
        case WIFI_EVENT_STA_CONNECTED:
        break;
        case WIFI_EVENT_STA_DISCONNECTED:
            if (wifi->flags&WIFI_FLG_VALID) {
                esp_wifi_connect();
            }
        break;
        default:
        break;
    }
}

void ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    switch (event_id)   {
        case IP_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "Wifi connected");
        break;
        default:
        break;
    }
}

void wifiTask(void *pvParameters)   {
    Wifi    *wifi=(Wifi *)pvParameters;
    wifi->disconnect_retry_cnt=0;
    wifi->flags=0;
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    wifi->events=xEventGroupCreate();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t init_cfg=WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&init_cfg);
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, (void *)wifi, &wifi->wifi_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, ip_event_handler, (void *)wifi, &wifi->ip_id);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    while (1)   {
        EventBits_t bits=xEventGroupWaitBits(wifi->events, 0x00ffffff, pdTRUE, pdFALSE, portMAX_DELAY);
    }
}

Wifi::Wifi()    {
    xTaskCreate(wifiTask, "WIFI_TASK", 4096, this, 4, &handle);
}

Wifi::~Wifi()   {
}

void Wifi::set_ap(char *ssid, char *password) {
    memset(&wifi_config, 0, sizeof(wifi_config_t));
    strcpy((char *)wifi_config.sta.ssid, ssid);
    strcpy((char *)wifi_config.sta.password, password);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    flags|=WIFI_FLG_VALID;
}

void Wifi::connect()    {
    esp_wifi_connect();
}

int32_t Wifi::load_settings()   {
    return 0;
}

int32_t Wifi::save_settings()   {
    return 0;
}


