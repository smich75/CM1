#ifndef OBJECT_H
#define OBJECT_H

#pragma once

#include <cstdint>
#include <cstddef>
#include <freertos/FreeRTOS.h>

class Object    {
public:
    Object();
    ~Object();
    void set_flags(uint32_t flgs);
    void add_flags(uint32_t mask);
    void clear_flags(uint32_t mask);
    uint32_t get_flags();
    int32_t lock(int32_t timeout);
    void unlock();

    uint32_t flags;

private:
    SemaphoreHandle_t   mutex;
};

#endif