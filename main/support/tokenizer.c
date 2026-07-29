#include <tokenizer.h>

static char *sTok;

char *tok(char *string, char del)   {
    char *t=NULL;
    if (string) sTok=string;
    t=sTok;
    while ((*sTok!=del)&&(*sTok!=0))    {
        sTok++;
    }
    if (*sTok)    {
        *sTok++=0x0;
    }   else {
        sTok=NULL;
        t=NULL;
    }
    return t;
}

bool getPair(char *string, char **key, char **val)    {
    *key=tok(string, '=');
    if (*key)    {
        *val=tok(NULL, ';');
        return true;
    }
    return false;
}
