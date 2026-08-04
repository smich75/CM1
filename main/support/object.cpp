#include "object.h"

Object::Object()    {
    mutex=xSemaphoreCreateMutex();
    flags=0;
}

Object::~Object()   {
    vSemaphoreDelete(mutex);
}

void Object::set_flags(uint32_t flgs)   {
    lock(portMAX_DELAY);
    flags=flgs;
    unlock();
}

void Object::add_flags(uint32_t mask)   {
    lock(portMAX_DELAY);
    flags|=mask;
    unlock();
}

void Object::clear_flags(uint32_t mask) {
    lock(portMAX_DELAY);
    flags&=(~mask);
    unlock();
}

uint32_t Object::get_flags()    {
    uint32_t ret;
    lock(portMAX_DELAY);
    ret=flags;
    unlock();
    return ret;
}

int32_t Object::lock(int32_t timeout)   {
    return xSemaphoreTake(mutex, timeout);
}

void Object::unlock()   {
    xSemaphoreGive(mutex);
}
