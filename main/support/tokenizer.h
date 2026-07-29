#ifdef __cplusplus
extern "C"  {
#endif

#include <stdint.h>
#include <stddef.h>

char *tok(char *string, char del);
bool getPair(char *string, char **key, char **val);

#ifdef __cplusplus
}
#endif