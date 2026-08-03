#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <module.h>

static const char *TAG="MAIN";
Module *module;

extern "C" void app_main(void) {
    module=new Module;
    while (1)   {
        printf("Main loop\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
