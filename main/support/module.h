#ifndef MODULE_H
#define MODULE_H

#pragma once

#include <cstdint>
#include <cstdio>
#include <wifi.h>
#include <freertos/FreeRTOS.h>
#include <cJSON.h>

class Module    {
public:
    Module();
    ~Module();
    void bsp_initialize();
    int32_t load_settings();
    int32_t save_settings();

    Wifi    *wifi;

    cJSON   *settings=NULL;
    int32_t display_brightness=100;

private:
    TaskHandle_t    handle;
};

#endif