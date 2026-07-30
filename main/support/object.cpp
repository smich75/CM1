#include "object.h"
#include <cstdio>

Object::Object()
{

}

Object::~Object()
{

}

int32_t Object::parse(char *buffer) {
    jsonObj=cJSON_Parse(buffer);
    cJSON *item;
    cJSON_ArrayForEach(item, jsonObj)   {
        printf("%s\n", item->string);
    }
    cJSON_Delete(jsonObj);
    return 0;
}
