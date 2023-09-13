#include "http_server.h"
#define HTTP_200_code 200
AsyncWebServer server(80);  // start server listening on port 80
void setup_http_server(){

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        // Code that holds the request
        Serial.println("Get received"); // Just for debug
        request->send(HTTP_200_code, "text/plain", "Here I am"); 
    });


    // Example of function that holds and HTTP_GET request on 192.168.4.1:80/get_data
    server.on("/get_data", HTTP_GET, [](AsyncWebServerRequest *request){
        // Code that holds the request
        Serial.println("Get received"); // Just for debug
        request->send(HTTP_200_code, "text/plain", "Here I am"); 
    });

    // Example of function that holds and HTTP_POST request on 192.168.4.1:80/set_data
    server.on("/set_data",
        HTTP_POST,
        [](AsyncWebServerRequest * request){},
        NULL,
        [](AsyncWebServerRequest * request, uint8_t *data, size_t len,
        size_t index, size_t total) {
            // Here goes the code to manage the post request
            // The data is received on 'data' variable
            // Parse data
            Serial.println("POST RECEIVED"); // Just for debug
            StaticJsonDocument<50> JSONBuffer; // create a buffer that fits for you
            deserializeJson(JSONBuffer, data); //Parse message
            // Serial.println(data);
            request->send(HTTP_200_code, "text/plain", "Some message");
    });

    server.begin();  // starts asyncrhonus controller

    Serial.println(WiFi.softAPIP());  // you can print you IP yo be sure that it is 192.168.4.1
}