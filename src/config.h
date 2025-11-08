#pragma once  // Prevents multiple inclusion errors

// Debugging macros
#define DEBUG_S true
#define DEBUG_PRINT(x)    if (DEBUG_S) Serial.print(x)
#define DEBUG_PRINTLN(x)  if (DEBUG_S) Serial.println(x)

// Pin definitions
#define LED_BUILTIN 2
#define LED_Status  4
#define LED_WiFi    5
#define BUTTON_START_PIN 18
#define BUTTON_STOP_PIN  19
#define BUZZER_PIN  23
#define SERVO_PIN   26

// Default servo position (use const, not global int)
const int SERVO_DEFAULT_POS = 0;  // 0 or 180 depending on build
