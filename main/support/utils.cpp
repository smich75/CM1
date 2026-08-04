#include "utils.h"

Utils   *Utils::ptr=NULL;

cJSON *Utils::load_file(char * filename)    {
    void *buffer=NULL;
    cJSON *tmp=NULL;
    FILE *f;
    f=fopen(filename, "r");
    if (f)  {
        int32_t buffer_size;
        fseek(f, 0, SEEK_END);
        buffer_size=ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer=malloc(buffer_size);
        if (buffer) {
            fread(buffer, buffer_size, 1, f);
            tmp=cJSON_ParseWithLength((char *)buffer, buffer_size);
            free(buffer);
        }
        fclose(f);
    }
    return tmp;
}