#ifndef THINQ_H
#define THINQ_H

#pragma once
#include <cstdint>

class ThinQ
{
public:
    ThinQ();
    ~ThinQ();
    const char *getEndpoint(int32_t epNum);
    void parseSettingsFile();
private:

};

#endif