#ifndef WIFI_H
#define WIFI_H

#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <freertos/FreeRTOS.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <object.h>
#include <utils.h>

//                              FLAGS
#define WIFI_FLG_STA_READY  BIT0
#define WIFI_FLG_VALID      BIT1
#define WIFI_FLG_CONNECTED  BIT2

//                              EVENT GROUP BITS
#define WIFI_TEST_BIT   BIT0

class Wifi : public Object {
public:
    Wifi();
    ~Wifi();

    void set_ap(char *ssid, char *password);
    void connect();
    void setup();
    int32_t load_config();
    int32_t save_config();

    uint8_t disconnection_reason;
    int32_t disconnect_retry_cnt;

    cJSON           *config;
    TaskHandle_t    handle;
    EventGroupHandle_t  events;
    esp_event_handler_instance_t wifi_id;
    esp_event_handler_instance_t ip_id;
    wifi_config_t   wifi_config;
};

#endif