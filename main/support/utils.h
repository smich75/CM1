#ifndef UTILS_H
#define UTILS_H

#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cJSON.h>

class Utils {
public:
    cJSON *load_file(char *filename);
    bool save_file(cJSON *data);

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