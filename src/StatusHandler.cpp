#include "StatusHandler.h"
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>

void StatusHandler::StatusOnStart() {
    // STATUS ON STARTUP
    // POWER STATUS
    digitalWrite(LED_BUILTIN, HIGH);
    Blynk.virtualWrite(V0, HIGH);
    _POWER_STATUS = 1;
    _PREV_POWER_STATUS = _POWER_STATUS;

    // WIFI STATUS
    if (Blynk.connected()) { // true
        digitalWrite(LED_WiFi, HIGH);
        Blynk.virtualWrite(V2, HIGH);
        _WIFI_STATUS = 1;
        _PREV_WIFI_STATUS = _WIFI_STATUS;
    } else {
        digitalWrite(LED_WiFi, LOW);
        Blynk.virtualWrite(V2, LOW);
        _WIFI_STATUS = 0;
        _PREV_WIFI_STATUS = _WIFI_STATUS;
    }
}

void StatusHandler::StatusUpdate() {
    // STATUS UPDATING
    // POWER STATUS
    if (_POWER_STATUS != _PREV_POWER_STATUS) {
        _PREV_POWER_STATUS = _POWER_STATUS; // update
        digitalWrite(LED_BUILTIN, _POWER_STATUS ? HIGH : LOW);
        Blynk.virtualWrite(V0, _POWER_STATUS ? HIGH : LOW);
    }
    // WIFI STATUS
    if (_WIFI_STATUS != _PREV_WIFI_STATUS) {
        _PREV_WIFI_STATUS = _WIFI_STATUS;
        digitalWrite(LED_WiFi, _WIFI_STATUS ? HIGH : LOW);
        Blynk.virtualWrite(V2, _WIFI_STATUS ? HIGH : LOW);
    }
}

void StatusHandler::FeedStatusReset(){
    Blynk.virtualWrite(V7, 0);
    Blynk.virtualWrite(V8, 0);
    Blynk.virtualWrite(V9, 0);
}
