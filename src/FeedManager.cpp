#include <Arduino.h>
#include "FeedManager.h"
#include "LCDManager.h"

LCDManager lcdManager;

// Constructor  
FeedManager::FeedManager(Servo& servo, int defaultPos, int buzzerPin, int ledPin) 
    : _servo(servo), 
      _defaultPos(defaultPos), 
      _buzzerPin(buzzerPin), 
      _ledPin(ledPin) 
    {
        currentFeedState.active = false;
        currentFeedState.startTime = 0;
        currentFeedState.duration = 0;
        currentFeedState.size = "";
}

void FeedManager::startFeed(String size, int duration, bool isTimer, bool isManual) {
    if (!currentFeedState.active) {
        currentFeedState.active = true;
        currentFeedState.startTime = millis(); // start counting from now
        currentFeedState.duration = duration;
        currentFeedState.size = size;

        digitalWrite(_buzzerPin, HIGH);
        digitalWrite(_ledPin, HIGH);
        DEBUG_PRINTLN(isTimer ? "TIMED FEEDING" : "FEEDING...");

        
        lcdManager.feedingMessage(isTimer);
        _servo.write(portion(size));
    }

    updateFeed();
}

void FeedManager::updateFeed() {
    if (!currentFeedState.active) {
        return;
    }

    if (millis() - currentFeedState.startTime >= currentFeedState.duration) {  
        digitalWrite(_buzzerPin, LOW);
        digitalWrite(_ledPin, LOW);
        _servo.write(_defaultPos);
        currentFeedState.active = false;
    }
}