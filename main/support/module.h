#ifndef MODULE_H
#define MODULE_H

#pragma once

#include <cstdint>
#include <cstdio>
#include <wifi.h>
#include <websocketserver.h>
#include <object.h>
#include <freertos/FreeRTOS.h>
#include <cJSON.h>

class Module: public Object  {
public:
    Module();
    ~Module();
    void bsp_initialize();
    int32_t load_config();
    int32_t save_config();

    Wifi    *wifi;
    WebSocketServer *ws_server;

    cJSON   *settings=NULL;
    int32_t display_brightness=100;

private:
    TaskHandle_t    handle;
};

#endif