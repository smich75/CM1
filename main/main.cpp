#include <stdio.h>
#include <esp_log.h>
#include <bsp/esp32_p4_wifi6_touch_lcd_7b.h>

static const char *TAG="MAIN";

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

esp_vfs_littlefs_conf_t littleFSConf={
    .base_path="/littlefs",
    .partition_label="storage",
    .format_if_mount_failed=1,  
    .dont_mount = false,
};

extern "C" void app_main(void) {
    //esp_log_level_set("*", ESP_LOG_NONE);
    
   //cJSON_InitHooks(&hooks);
    //mainEventGroup=xEventGroupCreate();
    //wifiEventGroup=xEventGroupCreate();
    esp_vfs_littlefs_register(&littleFSConf);
    bsp_display_start_with_config(&cfg);
    bsp_display_brightness_set(100);
    bsp_display_lock(0);
    ui_init();
    thinq.parseSettingsFile();
    bsp_display_unlock();
    wifi=new WIFI_IF();
    //while (!wifi->getFlags()&&WIFI_SCAN_LIST_READY);
    jsonTxt=loadJSON((char *)"/littlefs/wifi.json");
    char *ssid=NULL, *pwd=NULL;
    int32_t retry=0;
    if (jsonTxt)    {
        //printf("Current available ram: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
        //printf("Internal heap: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
        jsonObj=cJSON_Parse(jsonTxt);
        //printf("Available ram after json parse: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
        //printf("Internal heap: %d bytes\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
        item=cJSON_GetObjectItem(jsonObj, "ssid");
        ssid=cJSON_GetStringValue(item);
        item=cJSON_GetObjectItem(jsonObj, "password");
        pwd=cJSON_GetStringValue(item);
       /*wifiSetupConnection(ssid, pwd, NULL);
        wifiInit(wifiEventGroup);*/
        //vTaskDelay(pdMS_TO_TICKS(5000));
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
