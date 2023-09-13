#include <esp32cam.h>
#include <WiFi.h>
#include <WebServer.h>


static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);


WebServer streaming_server(80);


void stream_client() {
    streaming_server.handleClient();
}

void serveJpg()
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    // Serial.println("CAPTURE FAIL");
    streaming_server.send(503, "", "");
    return;
  }
  // Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                // static_cast<int>(frame->size()));
 
  streaming_server.setContentLength(frame->size());
  streaming_server.send(200, "image/jpeg");
  WiFiClient client = streaming_server.client();
  frame->writeTo(client);
}

void handleJpgLo()
{
  if (!esp32cam::Camera.changeResolution(loRes)) {
    // Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}
 
void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    // Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}
 
void handleJpgMid()
{
  if (!esp32cam::Camera.changeResolution(midRes)) {
    // Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}
 

 void video_stream() {
  streaming_server.on("/cam-lo.jpg", handleJpgLo);
  streaming_server.on("/cam-hi.jpg", handleJpgHi);
  streaming_server.on("/cam-mid.jpg", handleJpgMid);

  streaming_server.begin();
}

void print_link() {
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-lo.jpg");
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /cam-mid.jpg");
}