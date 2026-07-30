#ifndef OBJECT_H
#define OBJECT_H

#pragma once
#include <cstdint>
#include <cJSON.h>

class Object
{
public:
    Object();
    ~Object();
    int32_t parse(char *buffer);
    
private:
cJSON   *jsonObj;

};

#endif