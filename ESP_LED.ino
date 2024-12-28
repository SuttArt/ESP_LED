#include <Adafruit_NeoPixel.h>
#include "WebServerHandler.h"
#include "LogHandler.h"

#define LED_PIN 2      // GPIO2 (D4)
#define NUM_LEDS 80    // Кількість світлодіодів
#define BUTTON1_PIN 5  // GPIO5 (D1) - кнопка для вимкнення
#define BUTTON2_PIN 4  // GPIO4 (D2) - кнопка для зміни кольору

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Змінні для збереження стану стрічки
bool isStripOn = false; // Чи стрічка увімкнена
uint32_t currentColor = 0; // Поточний колір стрічки

void setup() {
  Serial.begin(115200);
  delay(1000); // Затримка для стабілізації
  
  // Ініціалізація логів
  initLog();
  writeToLog("Система стартує...");
  Serial.println("Система стартує...");

  // Налаштування кнопок
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  // Ініціалізація стрічки
  strip.begin();
  strip.show(); // Вимкнути всі світлодіоди

  // Ініціалізація Wi-Fi та веб-сервера
  initWiFi();
  initWebServer();

  // Вивід причини перезапуску
  Serial.print("Причина перезапуску: ");
  Serial.println(ESP.getResetReason());
  Serial.println("Стрічка готова до роботи!");
}

void loop() {
  server.handleClient();
  // Перевірка кнопки 1 (вимкнення стрічки)
  if (digitalRead(BUTTON1_PIN) == LOW) {
    turnOffStrip();
    delay(200); // Анти-дребезг
  }

  // Перевірка кнопки 2 (ввімкнення або зміна кольору)
  if (digitalRead(BUTTON2_PIN) == LOW) {
    changeColor();
    delay(200); // Анти-дребезг
  }
}

// Функція для вимкнення стрічки
void turnOffStrip() {
  if (isStripOn) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, 0); // Вимкнути світлодіоди
    }
    strip.show();
    isStripOn = false; // Стрічка вимкнена
    Serial.println("Стрічка вимкнена");
  }
}

// Функція для зміни кольору
void changeColor() {
  // Генерація нового кольору, якщо стрічка ввімкнена
  currentColor = strip.Color(random(0, 256), random(0, 256), random(0, 256));

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, currentColor);
  }
  strip.show();
  isStripOn = true; // Стрічка ввімкнена
  Serial.println("Колір змінено");
}
