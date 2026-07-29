#include "ThinQ.h"
#include <cstring>
#include <cstdio>
#include <tokenizer.h>

static char thinqSettings[]="PAT=;ClientID=;Endpoint=TestEndPoint;";
char *dup;

static const char *endpoints[]={
    "https://api-kic.lgthinq.com",
    "https://api-aic.lgthinq.com",
    "https://api-eic.lgthinq.com"
};

ThinQ::ThinQ()  {

}

ThinQ::~ThinQ() {

}

const char *ThinQ::getEndpoint(int32_t epNum)   {
    return endpoints[epNum];
}

void ThinQ::parseSettingsFile() {
    char *key;
    char *val;
    char *str=thinqSettings;
    while (getPair(str, &key, &val))  {
        str=NULL;
        printf("Key: %s - Val: %s\n", key, val);
    }
}
