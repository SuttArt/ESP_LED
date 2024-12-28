#include "LogHandler.h"

// Функція для отримання дати та часу
String getCurrentTimestamp() {
    // Приклад: Реалізувати отримання дати і часу через millis (або NTP)
    unsigned long now = millis() / 1000; // Поточний час у секундах
    int days = now / 86400;
    int hours = (now / 3600) % 24;
    int minutes = (now / 60) % 60;
    int seconds = now % 60;

    char timestamp[20];
    snprintf(timestamp, sizeof(timestamp), "%02d.%02d.%02d %02d:%02d:%02d", 
             28, 12, 24, hours, minutes, seconds); // Замініть "28.12.24" реальними значеннями, якщо є NTP

    return String(timestamp);
}

void initLog() {
    if (!LittleFS.begin()) {
        Serial.println("Не вдалося ініціалізувати LittleFS");
    } else {
        Serial.println("LittleFS успішно ініціалізовано");
    }
}

void writeToLog(String message) {
    if (!LittleFS.begin()) return;

    // Перевіряємо розмір файлу
    if (LittleFS.exists("/log.txt")) {
        File logFile = LittleFS.open("/log.txt", "r");
        if (logFile) {
            size_t fileSize = logFile.size();
            logFile.close();

            // Якщо файл перевищує розмір, очищуємо його
            if (fileSize >= MAX_LOG_SIZE) {
                Serial.println("Лог-файл перевищує розмір, очищення...");
                LittleFS.remove("/log.txt");
            }
        }
    }

    // Додаємо новий запис
    File logFile = LittleFS.open("/log.txt", "a");
    if (logFile) {
        // Форматування лог-запису
        String logEntry = getCurrentTimestamp() + "\n    " + message;
        logFile.println(logEntry);
        logFile.close();
        Serial.println("Записано в лог: " + logEntry);
    } else {
        Serial.println("Не вдалося відкрити лог-файл для запису");
    }
}

String readFromLog() {
    if (!LittleFS.begin()) return "Помилка ініціалізації LittleFS";

    if (!LittleFS.exists("/log.txt")) {
        return "Лог-файл не знайдено";
    }

    File logFile = LittleFS.open("/log.txt", "r");
    if (!logFile) {
        return "Не вдалося відкрити лог-файл для читання";
    }

    String logContent;
    while (logFile.available()) {
        logContent += logFile.readStringUntil('\n');
    }
    logFile.close();

    return logContent;
}
