#ifndef MODULE_H
#define MODULE_H

#pragma once

#include <cstdint>
#include <cstdio>
#include <wifi.h>
#include <freertos/FreeRTOS.h>

class Module    {
public:
    Module();
    ~Module();
    void bsp_initialize();
    int32_t load_settings(char *filename);
    int32_t save_settings(char *filename);

    Wifi    *wifi;

private:
    TaskHandle_t    handle;
};

#endif