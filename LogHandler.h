#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <Arduino.h>
#include <LittleFS.h>

#define MAX_LOG_SIZE 1024 * 10  // Максимальний розмір лог-файлу (10 KB)

// Функції для роботи з логами
void initLog();                  // Ініціалізація файлової системи
void writeToLog(String message); // Запис у лог
String readFromLog();            // Читання з логу

#endif // LOGHANDLER_H
