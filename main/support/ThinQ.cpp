#include "ThinQ.h"
#include <cstring>
#include <cstdio>

static char thinqSettings[]="PAT=;ClientID=;";
char *dup;

static const char *endpoints[]={
    "https://api-kic.lgthinq.com",
    "https://api-aic.lgthinq.com",
    "https://api-eic.lgthinq.com"
};

ThinQ::ThinQ()  {

}

ThinQ::~ThinQ()
{

}

const char *ThinQ::getEndpoint(int32_t epNum)   {
    return endpoints[epNum];
}

void ThinQ::parseSettingsFile() {
    dup=strdup(thinqSettings);
    char *token=strsep((char **)&dup, "=");
    while (token) {
        printf("Token: %s\n", token);
        token=strsep((char **)&dup, ";");
        //if (token==NULL)    printf("Empty value\n");
            /*else*/ printf("Val: %s\n", token);
        token=strsep((char **)&dup, "=");
    }
}
