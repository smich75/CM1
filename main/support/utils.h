#ifndef UTILS_H
#define UTILS_H

#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cJSON.h>

class Utils {
public:
    cJSON *load_config_file(char *filename);
    bool save_config_file(cJSON *data, char *filename);

    static Utils *get() {
        if (!ptr)   {
            ptr=new Utils;
        }
        return ptr;
    }
    ~Utils()    {};

private:
    static Utils *ptr;
    Utils() {};
};

#endif