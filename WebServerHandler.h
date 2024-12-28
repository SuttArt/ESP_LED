#ifndef WEBSERVERHANDLER_H
#define WEBSERVERHANDLER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

// Налаштування Wi-Fi
extern const char* ssid;
extern const char* password;

// Об'єкт веб-сервера
extern ESP8266WebServer server;

// Функції для роботи з веб-сервером
void initWiFi();
void initWebServer();
void handleRoot();

// REST API
void handleChangeColor();
void handleTurnOffStrip();



#endif // WEBSERVERHANDLER_H
