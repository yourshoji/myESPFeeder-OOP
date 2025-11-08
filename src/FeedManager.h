#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include <TimeManager.h>
#include <config.h>

class FeedManager {
private:
    // Private members and methods for internal use

    // Hardware references
    Servo& _servo; // reference to the servo object
    int _defaultPos; // default servo position
    int _buzzerPin; // buzzer pin
    int _ledPin; // status LED pin

    struct FeedTimer {
        int hr;
        int min;
        int sec;
        String size;
        int duration;
        bool triggered;
    };
    
    struct FeedState {
        bool active;
        unsigned long startTime;
        int duration;
        String size;
    };

    FeedState currentFeedState; // create an object to track current feed state

    FeedTimer FeedSchedule[3] = {
    // {14, 43, 0, "small", 3000, false},
    // {14, 45, 0, "medium", 5000, false},
    // {14, 47, 0, "big", 7000, false} examples
};
    
public:
    // Constructor (reference to servo object and pin configurations)
    FeedManager(
        Servo& servo, 
        int defaultPos = SERVO_DEFAULT_POS, 
        int buzzerPin = BUZZER_PIN, 
        int ledPin = LED_Status
    );

    // Public methods/functions
    void startFeed(String size, int duration, bool isTimer, bool isManual);

    void updateFeed();

    void scheduleFeed(TimeManager::rtc_src rtcTimer);    

    void resetSchedule();

    void resetArray();
    
    void resetFeedStatus();

    String readArray(int x);

    int portion(String size){ // helper to determine servo position based on size
    if (size == "small") { return 30; }
    if (size == "medium") {return 60;} 
    if (size == "big") {return 90;} 
    return _defaultPos; // fallback to default position
}
};