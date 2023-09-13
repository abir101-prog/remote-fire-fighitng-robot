#ifndef VIDEO_SERVER_H
#define VIDEO_SERVER_H

#include <esp32cam.h>
#include <WiFi.h>
#include <WebServer.h>


void video_stream();
void client_handler();

void serveJpg();

void handleJpgLo();
void handleJpgHi();
void handleJpgMid();

#endif