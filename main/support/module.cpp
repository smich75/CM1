#include "module.h"

#include <esp_littlefs.h>
#include <bsp/esp32_p4_wifi6_touch_lcd_7b.h>

static const char settings_file[]="/littlefs/settings.json";

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

static esp_vfs_littlefs_conf_t little_fs_conf={
    .base_path="/littlefs",
    .partition_label="storage",
    .format_if_mount_failed=1
};


Module::Module()    {
    handle=xTaskGetCurrentTaskHandle();
    bsp_initialize();
    load_settings();
    save_settings();

    wifi=new Wifi;
}

Module::~Module()   {
}

void Module::bsp_initialize()   {
    esp_vfs_littlefs_register(&little_fs_conf);
    bsp_display_start_with_config(&cfg);
    bsp_display_brightness_set(100);
}

int32_t Module::load_settings()
{
    FILE *f=NULL;
    int32_t bufferSize;
    void    *buffer;
    int32_t ret_val=0;
    f=fopen(settings_file, "r");
    if (f)  {
        fseek(f, 0, SEEK_END);
        bufferSize=ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer=malloc(bufferSize);
        if (buffer) {
            fread(buffer, bufferSize, 1, f);
            settings=cJSON_ParseWithLength((char *)buffer, bufferSize);
            if (settings)   {
                char *parsed_file=cJSON_Print(settings);
                printf("%s\n", parsed_file);
                ret_val=1;
            }
            free(buffer);
        }
        fclose(f);
    }
    return ret_val;
}

int32_t Module::save_settings() {
    cJSON   *tmp;
    FILE    *f;
    int32_t ret_val=0;
    if (settings)   {
        cJSON_free(settings);
    }
    settings=cJSON_CreateObject();
    if (settings)   {
        tmp=cJSON_AddObjectToObject(settings, "display");
        tmp=cJSON_AddNumberToObject(tmp, "brightness", (double)display_brightness);

        f=fopen(settings_file, "w+");
        if (f)  {
            char *buffer=cJSON_Print(settings);
            fputs(buffer, f);
            fclose(f);
            return ret_val=1;
        }
    }
    return ret_val;
}


