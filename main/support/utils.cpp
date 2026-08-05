#include "utils.h"

Utils   *Utils::ptr=NULL;

cJSON *Utils::load_config_file(char * filename)    {
    void *buffer=NULL;
    cJSON *tmp=NULL;
    FILE *f;
    f=fopen(filename, "r");
    if (f)  {
        int32_t buffer_size;
        fseek(f, 0, SEEK_END);
        buffer_size=ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer=calloc(1, buffer_size+1);
        if (buffer) {
            fread(buffer, buffer_size, 1, f);
            tmp=cJSON_Parse((char *)buffer);
            free(buffer);
        }
        fclose(f);
    }
    return tmp;
}

bool Utils::save_config_file(cJSON * data, char *filename)  {
    FILE *f=NULL;
    f=fopen(filename, "w+");
    if (f)  {
        char *buffer=cJSON_Print(data);
        fputs(buffer, f);
        fclose(f);
        return true;
    }
    return false;
}
