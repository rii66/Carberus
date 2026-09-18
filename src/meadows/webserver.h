#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

extern WebServer server;
extern WebSocketsServer webSocket;

void initWebServer();

#endif
