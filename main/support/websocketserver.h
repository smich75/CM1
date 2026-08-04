#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#pragma once
#include <cstdint>
#include <cstddef>
#include <esp_http_server.h>
#include <esp_log.h>

class WebSocketServer   {
public:
    WebSocketServer();
    ~WebSocketServer();

private:
    httpd_handle_t  server_handle;
};

#endif