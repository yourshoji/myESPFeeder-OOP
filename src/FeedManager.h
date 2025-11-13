#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <RtcDS1302.h>
#include "config.h" // for WEB_APP variable

enum FeedSize {
    SMALL,
    MEDIUM,
    BIG
};

struct FeedTimer {
    int hr;
    int min;
    int sec;
    FeedSize size;
    int duration;
    bool triggered;
};

struct RtcStatus {
    int rtc_hr;
    int rtc_min;
    int rtc_sec;
};

struct FeedState {
    bool active;
    unsigned long startTime;
    int duration;
    FeedSize size;
};


class FeedManager {
    private:
        // Private members and methods for internal use

        // Hardware references
        Servo& _SERVO; // reference to the servo object
        LiquidCrystal_I2C& _LCD;
        RtcDS1302<ThreeWire>& _RTC;
        int _SERVO_DEFAULT_POS; // default servo position
        int _BUZZER_PIN; // buzzer pin
        int _LED_PIN; // status LED pin

        unsigned long _delayStartFeed = 0;
        bool _delayingFeed = false;
        
        unsigned long _delayStartReset = 0;
        bool _delayingReset = false;

        FeedState _currentFeedState; // create an object to track current feed state
        bool _eStop = false;
        bool _prev_eStop = false;

        int portion(FeedSize size); // helper to determine servo position based on size
        bool betterDelay(unsigned long delayDuration, unsigned long &delayStart, bool &delaying);
        // "&" is a reference to the real variable, not just a copy
        String rtc_strTimer();
        
    public:
        // Constructor (reference to servo object and pin configurations)
        // Make each room (servo, lcd, rtc, etc.) functional when called
        FeedManager(
            Servo& SERVO,
            LiquidCrystal_I2C& LCD,
            RtcDS1302<ThreeWire>& RTC,
            int LOCAL_BUZZER_PIN,
            int LOCAL_LED_PIN,
            int SERVO_DEFAULT_POS 
        );

        // Public methods/functions
        void startFeed(FeedSize size, int duration, bool isTimer, bool isManual);

        void updateFeed();

        void scheduleFeed(FeedTimer* FeedingSchedule, int ScheduleCounter, const RtcStatus& status);    

        void resetSchedule(FeedTimer* FeedingSchedule, int ScheduleCounter);

        void delayTillReset(FeedTimer* FeedingSchedule, int ScheduleCounter);

        void sheetLogger(const char* WEB_APP, String val1, String val2, String val3);

        void setEStop(bool state);
        bool getEStop();

        // DEBUG 
        String readSchedule(FeedTimer* FeedingSchedule, int ScheduleCounter) const;
        
        
    };