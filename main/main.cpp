#include <stdio.h>
#include <esp_log.h>
#include <bsp/esp32_p4_wifi6_touch_lcd_7b.h>
#include <ui.h>
#include <screens.h>
#include <actions.h>
#include <freertos/FreeRTOS.h>
#include <wifi.h>
#include <esp_hosted.h>
#include <thinq.h>
#include <utils.h>
#include <object.h>
#include <esp_littlefs.h>
#include <cJSON.h>


static const char *TAG="MAIN";
extern objects_t objects;

void handleMainEvents(uint32_t timeout);
void handleWifiEvents(uint32_t timeout);
void uiRefresh();

static bsp_display_cfg_t  cfg={
    .lvgl_port_cfg=ESP_LVGL_PORT_INIT_CONFIG(),
    .buffer_size=1024*60,
    .double_buffer=1,
    .flags={
        .buff_dma=1,
        .buff_spiram=0,
        .sw_rotate=0
    }
};

static EventGroupHandle_t mainEventGroup;
static EventGroupHandle_t wifiEventGroup;
static EventBits_t mainBits, wifiBits;

void action_wifi_scan(lv_event_t *e)  {
    ESP_LOGI(TAG, "Action scan");
    lv_obj_add_state(objects.scan, LV_STATE_DISABLED);
    wifiScan();
}

void handleMainEvents(uint32_t timeout) {
    mainBits=xEventGroupWaitBits(mainEventGroup, 0x00ffffff, pdTRUE, pdFALSE, timeout);
}

void handleWifiEvents(uint32_t timeout) {
    wifiBits=xEventGroupWaitBits(wifiEventGroup, 0x00ffffff, pdTRUE, pdFALSE, timeout);
    if (wifiBits&WIFI_CONNECTED_BIT)  {
    }
    if (wifiBits&WIFI_FAIL_BIT) {
    }
    if (wifiBits&WIFI_SCAN_DONE)    {
        //ESP_LOGI(TAG, "Scan done");
        lv_obj_clear_state(objects.scan, LV_STATE_DISABLED);
        //ESP_LOGI(TAG, "Creating scan list");
        uint16_t apNum=wifiGetAPNum();
        int32_t row=0;
        uint16_t width=lv_obj_get_width(objects.sta_scan_list);
        if (apNum>0) lv_table_set_row_cnt(objects.sta_scan_list, apNum);
        lv_table_set_col_cnt(objects.sta_scan_list, 1);
        lv_table_set_col_width(objects.sta_scan_list, 0, width-4);//-48);
        //lv_table_set_col_width(objects.sta_scan_list, 1, 48);
        while (apNum>0) {
            char *ssid=(char *)wifiGetAPRecord()->ssid;
            //ESP_LOGI(TAG, "Found AP: %s", ssid);
            lv_table_set_cell_value(objects.sta_scan_list, row, 0, ssid);
            row++;
            apNum--;
        }
    }
}

void uiRefresh()    {
    bsp_display_lock(0);
    ui_tick();
    bsp_display_unlock();
}

void action_select_ssid(lv_event_t *e) {
    uint16_t row, col;
    const char *val;
    lv_obj_t *obj=lv_event_get_target(e);
    lv_table_get_selected_cell(obj, &row, &col);
    ESP_LOGI(TAG, "Selected %d, %d", row, col);
    val=lv_table_get_cell_value(obj, row, col);
    lv_textarea_set_text(objects.ssid_text_area, val);
}

ThinQ thinq;

esp_vfs_littlefs_conf_t littleFSConf={
    .base_path="/littlefs",
    .partition_label="storage",
    .format_if_mount_failed=1,  
    .dont_mount = false,
};

char *jsonTxt;
cJSON *jsonObj;
cJSON *item;

static void* cjson_spiram_malloc(size_t size) {
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
}

static void cjson_spiram_free(void* ptr) {
    heap_caps_free(ptr);
}

cJSON_Hooks hooks = {
    .malloc_fn = cjson_spiram_malloc,
    .free_fn = cjson_spiram_free
};

extern "C" void app_main(void) {
    //esp_log_level_set("*", ESP_LOG_NONE);
    
   //cJSON_InitHooks(&hooks);
    mainEventGroup=xEventGroupCreate();
    wifiEventGroup=xEventGroupCreate();
    esp_vfs_littlefs_register(&littleFSConf);
    bsp_display_start_with_config(&cfg);
    bsp_display_brightness_set(100);
    bsp_display_lock(0);
    ui_init();
    thinq.parseSettingsFile();
    bsp_display_unlock();
    jsonTxt=loadJSON((char *)"/littlefs/wifi.json");
    char *ssid=NULL, *pwd=NULL;
    if (jsonTxt)    {
        printf("Current available ram: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
        printf("Internal heap: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
        jsonObj=cJSON_Parse(jsonTxt);
        printf("Available ram after json parse: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
        printf("Internal heap: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
        item=cJSON_GetObjectItem(jsonObj, "ssid");
        ssid=cJSON_GetStringValue(item);
        item=cJSON_GetObjectItem(jsonObj, "password");
        pwd=cJSON_GetStringValue(item);
        printf("Connecting to %s with password=%s\n", ssid, pwd);
        wifiSetupConnection(ssid, pwd, NULL);
        wifiInit(wifiEventGroup);
        cJSON_Delete(jsonObj);
        free(jsonTxt);
    }

    while (1)   {
        handleMainEvents(0);
        handleWifiEvents(0);
        uiRefresh();
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
