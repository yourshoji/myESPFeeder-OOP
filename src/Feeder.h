#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>

class FeedManager {
private:
    // Private members and methods for internal use

    // Hardware references
    Servo& _servo;
    int _defaultPos; // default servo position
    int _buzzerPin; // buzzer pin
    int _ledPin; // status LED pin
    int _servoPin; // pin where servo is attached

    // State variables
    bool _active;
    unsigned long _startTime;
    int _duration;

    int portion(String size){ // helper to determine servo position based on size
        if (size == "small") { return 30; }
        if (size == "medium") {return 60;} 
        if (size == "big") {return 90;} 
        return _defaultPos; // fallback to default position
    }
    
public:
    // Constructor
    FeedManager(Servo& servo, int defaultPos, int buzzerPin, int ledPin, int servoPin);

    void startFeed(String size, int delay, bool isTimer, bool isManual);

    void updateFeed();
};