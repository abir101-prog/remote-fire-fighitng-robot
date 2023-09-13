#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"


#ifndef _HTTP_SERVER_
#define _HTTP_SERVER_

extern bool idle;
extern bool fire;
extern int total_step_count;
extern int FLASH_LIGHT;

void setup_async_server();

#endif