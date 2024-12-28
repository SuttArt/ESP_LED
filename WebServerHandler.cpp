#include "WebServerHandler.h"

// Задайте SSID і пароль для Wi-Fi
const char* ssid = "WIFI_ID";
const char* password = "WIFI_PASSWORD";

// Ініціалізація веб-сервера
ESP8266WebServer server(80);

void initWiFi() {
    const unsigned long timeout = 15000; // Таймаут у мілісекундах (15 секунд)
    unsigned long startAttemptTime = millis();

    WiFi.begin(ssid, password);
    Serial.print("Підключення до Wi-Fi...");

    // Чекаємо на підключення або завершення таймауту
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi підключено!");
        Serial.print("IP-адреса: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nНе вдалося підключитися до Wi-Fi!");
        // Додаткові дії у разі невдачі
        WiFi.disconnect(); // Відключаємо Wi-Fi
        Serial.println("Веб-сервер не буде запущено.");
    }
}


void initWebServer() {
    if (WiFi.status() == WL_CONNECTED) {
        server.on("/", handleRoot);

        // REST API
        server.on("/api/color", HTTP_POST, handleChangeColor); // POST: змінити колір
        server.on("/api/off", HTTP_POST, handleTurnOffStrip); // POST: вимкнути стрічку

        server.begin();
        Serial.println("Веб-сервер запущено!");
    } else {
        Serial.println("Веб-сервер не запущено через відсутність Wi-Fi підключення.");
    }
}

// Головна сторінка
void handleRoot() {
    Serial.println("Отримано запит на головну сторінку.");
    if (!LittleFS.begin()) {
        Serial.println("Не вдалося ініціалізувати LittleFS");
        server.send(500, "text/plain; charset=utf-8", "Помилка файлової системи");
        return;
    }

    File file = LittleFS.open("/index.html", "r");
    if (!file) {
        server.send(404, "text/plain; charset=utf-8", "Файл не знайдено");
        return;
    }

    server.streamFile(file, "text/html; charset=utf-8");
    file.close();
}


void handleChangeColor() {    
    Serial.println("Запит на зміну кольору стрічки.");
    // Виклик функції changeColor з .ino
    extern void changeColor();
    changeColor();

    server.send(200, "application/json", "{\"status\": \"success\", \"action\": \"color_changed\"}");
}


void handleTurnOffStrip() {
    Serial.println("Запит на вимкнення стрічки.");
    // Виклик функції turnOffStrip з .ino
    extern void turnOffStrip();
    turnOffStrip();

    server.send(200, "application/json", "{\"status\": \"success\", \"action\": \"strip_off\"}");
}
