#pragma once // Prevents errors if included multiple times

#define DEBUG_S true
#define DEBUG_PRINT(x) if (DEBUG_S) Serial.print(x)
#define DEBUG_PRINTLN(x) if (DEBUG_S) Serial.println(x)

#define LED_BUILTIN 2
#define LED_Status 4
#define LED_WiFi 5
#define buttonStartPin 18
#define buttonStopPin 19
#define buzzerPin 23
#define servoPin 26

int servo_def_pos = 0; // 0 or 180, depends on the build
