#include <utils.h>

char *loadJSON(char *filename)      {
    FILE *f;
    int32_t size;
    void *buffer=NULL;
    f=fopen(filename, "r");
    if (f)  {
        fseek(f, 0, SEEK_END);
        size=ftell(f);
        fseek (f, 0, SEEK_SET);
        buffer=malloc(size);
        if (buffer) {
            fread(buffer, size, 1, f);
        }
        fclose(f);
    }
    return (char *)buffer;
}
