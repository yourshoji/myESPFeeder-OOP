#include "LCDManager.h"
#include <Arduino.h>

    void LCDManager::initialize() {
        Wire.begin(21, 22); // set SDA and SCLK
        _lcd.init(); // initialize the lcd 
        _lcd.backlight(); // turn on backlight
        _lcd.setCursor(0,0);
        _lcd.print("INITIALIZING...");
        delay(3000);
        _lcd.clear();        
    }

    void LCDManager::wifiConnecting() {
        _lcd.setCursor(0,0);
        _lcd.print("CONNECTING...");
        delay(3000);
        _lcd.clear();
    }

    void LCDManager::allSetupDone() {
        _lcd.setCursor(0,0);
        _lcd.print("ALL SETUP DONE!");
        delay(3000);
        _lcd.clear();
    }

    void LCDManager::feedingMessage(bool isTimer) {
        _lcd.setCursor(0,0);
        _lcd.print(isTimer ? "TIMED FEEDING" : "FEEDING...");
        _lcd.setCursor(0,1);
        _lcd.print("rtc_strTimer"); // Placeholder for actual timer string
    }
