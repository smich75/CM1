#include "thinq.h"
#include <cstring>
#include <cstdio>
#include <esp_http_server.h>

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
}
