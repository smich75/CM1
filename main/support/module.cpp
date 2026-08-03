#include "module.h"
#include <cJSON.h>

#include <esp_littlefs.h>
#include <bsp/esp32_p4_wifi6_touch_lcd_7b.h>

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
    .format_if_mount_failed=1,  
    .dont_mount = false,
};


Module::Module()    {
    handle=xTaskGetCurrentTaskHandle();
    bsp_initialize();
}

Module::~Module()   {
}

void Module::bsp_initialize()   {
    esp_vfs_littlefs_register(&little_fs_conf);
    bsp_display_start_with_config(&cfg);
    bsp_display_brightness_set(100);
}
