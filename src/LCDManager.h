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

    void initialize();

    void wifiConnecting();

    void allSetupDone();

    void feedingMessage(bool isTimer);

};
