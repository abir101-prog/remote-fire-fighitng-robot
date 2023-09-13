#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include <esp32cam.h>

// just check if video works first:
AsyncWebServer server(80);

static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);

void serveJpg(AsyncWebServerRequest *request)
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    request->send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));
 
//   request->contentLength(); //frame->size());
//   request->send(200, "image/jpeg");
//   AsyncClient client = request->client();
//   frame->writeTo(client);
AsyncWebServerResponse *response = request->beginResponse_P(200, "image/jpeg", frame->writeTo());
response->addHeader("Content-Disposition", "inline; filename=cam-lo.jpg");
request->send(response);
}
 

// void handleJpgLo()
// {
//   if (!esp32cam::Camera.changeResolution(loRes)) {
//     Serial.println("SET-LO-RES FAIL");
//   }
//   serveJpg();
// }
 
// void handleJpgHi()
// {
//   if (!esp32cam::Camera.changeResolution(hiRes)) {
//     Serial.println("SET-HI-RES FAIL");
//   }
//   serveJpg();
// }
 
// void handleJpgMid()
// {
//   if (!esp32cam::Camera.changeResolution(midRes)) {
//     Serial.println("SET-MID-RES FAIL");
//   }
//   serveJpg();
// }
 


void server_setup() {
  server.on("/cam-lo.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  
  serveJpg(request);
  });
//   server.on("/cam-hi.jpg", handleJpgHi);
//   server.on("/cam-mid.jpg", handleJpgMid);


  server.begin();
}