#include "websocketserver.h"
#include <cstdio>

static const char *TAG="WEBSOCKETSERVER.CPP";
static httpd_config_t   cfg=HTTPD_DEFAULT_CONFIG();

static esp_err_t ws_handler(httpd_req_t *req)   {
    httpd_ws_frame_t ws_pkt;
    uint8_t *buf=NULL;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.type=HTTPD_WS_TYPE_TEXT;
    /* Set max_len = 0 to get the frame len */
    esp_err_t ret=httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed to get frame len with %d", ret);
        return ret;
    }
    ESP_LOGI(TAG, "frame len is %d", ws_pkt.len);
    if (ws_pkt.len) {
        /* ws_pkt.len + 1 is for NULL termination as we are expecting a string */
        buf=(uint8_t *)calloc(1, ws_pkt.len + 1);
        if (buf == NULL) {
            ESP_LOGE(TAG, "Failed to calloc memory for buf");
            return ESP_ERR_NO_MEM;
        }
        ws_pkt.payload = buf;
        /* Set max_len = ws_pkt.len to get the frame payload */
        ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
            free(buf);
            return ret;
        }
        printf("Got packet with message: %s\n", ws_pkt.payload);
    }
    ESP_LOGI(TAG, "Packet type: %d", ws_pkt.type);
    /*if (ws_pkt.type == HTTPD_WS_TYPE_TEXT &&
        ws_pkt.payload != NULL) {
        if (strncmp((char *)ws_pkt.payload, "Trigger async", strlen("Trigger async")) == 0) {
            free(buf);
            return trigger_async_send(req->handle, req);
        } else if (strncmp((char *)ws_pkt.payload, "Ping", strlen("Ping")) == 0) {
            free(buf);
            return trigger_ping_send(req->handle, req);
        }
    }*/

    /*ret = httpd_ws_send_frame(req, &ws_pkt);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_send_frame failed with %d", ret);
    }*/
    free(buf);
    return ret;
}


static const httpd_uri_t ws = {
        .uri        = "/ws",
        .method     = HTTP_GET,
        .handler    = ws_handler,
        .user_ctx   = NULL,
        .is_websocket = true
};

WebSocketServer::WebSocketServer()  {
    httpd_start(&server_handle, &cfg);
    httpd_register_uri_handler(server_handle, &ws);
}

WebSocketServer::~WebSocketServer() {

}