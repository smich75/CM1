#include "wifi.h"

static const char *TAG="WIFI.CPP";
static const char settings_file[]="/littlefs/wifi.json";

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    Wifi *wifi=(Wifi *)arg;
    switch (event_id)   {
        case WIFI_EVENT_STA_START:
            wifi->add_flags(WIFI_FLG_STA_READY);
            if (wifi->get_flags()&WIFI_FLG_VALID) {
                esp_wifi_connect();
            }
        break;
        case WIFI_EVENT_STA_CONNECTED:
        break;
        case WIFI_EVENT_STA_DISCONNECTED:
            wifi->disconnection_reason=((wifi_event_sta_disconnected_t *)event_data)->reason;
            ESP_LOGI(TAG, "Disconnection reason: %d", wifi->disconnection_reason);
            if (wifi->disconnection_reason==WIFI_REASON_AUTH_FAIL)  {
                wifi->clear_flags(WIFI_FLG_VALID|WIFI_FLG_CONNECTED);
            }   else    {
                wifi->clear_flags(WIFI_FLG_CONNECTED);
            }
            if (wifi->get_flags()&WIFI_FLG_VALID) {
                esp_wifi_connect();
            }
        break;
        default:
        break;
    }
}

void ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    Wifi *wifi=(Wifi *)arg;
    switch (event_id)   {
        case IP_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "Wifi connected");
            wifi->add_flags(WIFI_FLG_CONNECTED);
        break;
        default:
        break;
    }
}

void Wifi::setup()   {
    disconnect_retry_cnt=0;
    flags=0;
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    events=xEventGroupCreate();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t init_cfg=WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&init_cfg);
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, (void *)this, &wifi_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, ip_event_handler, (void *)this, &ip_id);
    esp_wifi_set_mode(WIFI_MODE_STA);
    load_config();
    esp_wifi_start();
}

Wifi::Wifi()    {
    setup();
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

int32_t Wifi::load_config()   {
    config=Utils::get()->load_file("/littlefs/wifi.json");
    if (config) {
        cJSON *ssid=cJSON_GetObjectItem(config, "ssid");
        cJSON *password=cJSON_GetObjectItem(config, "password");
        if (ssid&&password) {
            set_ap(cJSON_GetStringValue(ssid), cJSON_GetStringValue(password));
            return 1;
        }
    }
    return 0;
}

int32_t Wifi::save_config()   {
    return 0;
}


