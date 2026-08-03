#ifndef WIFI_H
#define WIFI_H

#pragma once
#include <cstdint>
#include <cstdio>

class Wifi  {
public:
    Wifi();
    ~Wifi();

    int32_t load_settings();
    int32_t save_settings();

private:

};

#endif