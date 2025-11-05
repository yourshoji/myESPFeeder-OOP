#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "TimeManager.h"

class LCDManager {
private:
    LiquidCrystal_I2C& _lcd;

public:
// Constructor with member initializer list (reference to LCD object)
    LCDManager(LiquidCrystal_I2C& lcd): _lcd(lcd){}

    void initialize() {
        _lcd.init(); // initialize the lcd 
        _lcd.backlight(); // turn on backlight
        _lcd.setCursor(0,0);
        _lcd.print("INITIALIZING...");
        delay(3000);
        _lcd.clear();        
    }

    void wifiConnecting() {
        _lcd.setCursor(0,0);
        _lcd.print("CONNECTING...");
        delay(3000);
        _lcd.clear();
    }

    void allSetupDone() {
        _lcd.setCursor(0,0);
        _lcd.print("ALL SETUP DONE!");
        delay(3000);
        _lcd.clear();
    }

    void feedingMessage(bool isTimer) {
        _lcd.setCursor(0,0);
        _lcd.print(isTimer ? "TIMED FEEDING" : "FEEDING...");
        _lcd.setCursor(0,1);
        _lcd.print("rtc_strTimer"); // Placeholder for actual timer string
    }

};
