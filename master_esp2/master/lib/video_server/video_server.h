#ifndef VIDEO_SERVER_H
#define VIDEO_SERVER_H

#include <esp32cam.h>
#include <WiFi.h>
#include <WebServer.h>


void video_stream();
void stream_client();

void serveJpg();

void handleJpgLo();
void handleJpgHi();
void handleJpgMid();

void print_link();

#endif