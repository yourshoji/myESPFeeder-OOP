#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

#include "EmergencyStop.h"
#include "config.h"
#include "credentials.h"
#include "TimeManager.h"
#include "LCDManager.h"
#include "FeedManager.h"
#include "BlynkManager.h"
#include "StatusHandler.h"

// -------------------- Global Variables --------------------
// LCD config
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RTC config
ThreeWire myWire(12, 27, 13);
RtcDS1302<ThreeWire> Rtc(myWire);

// Servo config
Servo myServo;

// Objects
LCDManager lcdManager(lcd);
TimeManager timeManager(Rtc);
EmergencyStop eStop(BUTTON_STOP_PIN, myServo);
StatusHandler statusHandler;
BlynkManager blynkManager;
FeedManager feedManager(
    myServo, 
    lcd, 
    Rtc, 
    BUZZER_PIN, 
    LED_Status,
    SERVO_DEFAULT_POS
);

// Feed schedule
FeedTimer FeedingSchedule[3]; // a schedule array, adjustable via Blynk
int FeedCounter = 0;
int ScheduleCounter = sizeof(FeedingSchedule) / sizeof(FeedingSchedule[0]);

// Time status
TimeManager::rtc_src status;

// -------------------- Helper Functions --------------------
void handleScheduleTime(FeedTimer &t, int totalSeconds) {
    int hr = totalSeconds / 3600;
    int min = (totalSeconds % 3600) / 60;
    int sec = totalSeconds % 60;
    t.hr = hr;
    t.min = min;
    t.sec = sec;
    t.triggered = false;
    FeedCounter++;
    DEBUG_PRINTLN("Counter: " + String(FeedCounter));
}

void handleScheduleSize(FeedTimer &t, int sizeParam) {
    switch (sizeParam) {
        case 0: t.size = SMALL; t.duration = 2000; break;
        case 1: t.size = MEDIUM; t.duration = 4000; break;
        case 2: t.size = BIG; t.duration = 6000; break;
    }
}

// -------------------- Blynk Handlers --------------------
BLYNK_CONNECTED() {
    blynkManager.BlynkSyncVirtualPins();
}

// Manual Feed Button
BLYNK_WRITE(V3) {
    if (param.asInt()) feedManager.startFeed(BIG, 5000, false, true);
}

// Schedule Handlers
BLYNK_WRITE(V4) { handleScheduleTime(FeedingSchedule[0], param[0].asInt()); }
BLYNK_WRITE(V5) { handleScheduleTime(FeedingSchedule[1], param[0].asInt()); }
BLYNK_WRITE(V6) { handleScheduleTime(FeedingSchedule[2], param[0].asInt()); }

// Reset and Read Schedule Buttons
BLYNK_WRITE(V10) {
    if (param.asInt()) {
        feedManager.resetSchedule(FeedingSchedule, ScheduleCounter);
        feedManager.readSchedule(FeedingSchedule, ScheduleCounter);
    }
}

// Read Schedule Button (via CMD)
BLYNK_WRITE(V11) {
    if (param.asInt()) {
        String msg = "Schedule:\n";
        for (int i = 0; i < ScheduleCounter; i++) {
            FeedTimer &t = FeedingSchedule[i];
            msg += String(i + 1) + ") " +
                   String(t.hr) + ":" + String(t.min) + ":" + String(t.sec) +
                   " | " + t.size +
                   " | " + String(t.duration / 1000) + "s\n";
        }
        DEBUG_PRINT(msg);
        Blynk.virtualWrite(V12, msg);
    }
}

// Schedule Size Handlers (1-3)
BLYNK_WRITE(V13) { handleScheduleSize(FeedingSchedule[0], param.asInt()); }
BLYNK_WRITE(V14) { handleScheduleSize(FeedingSchedule[1], param.asInt()); }
BLYNK_WRITE(V15) { handleScheduleSize(FeedingSchedule[2], param.asInt()); }

// Emergency Stop Handler
BLYNK_WRITE(V16) { eStop.E_STOP(param.asInt()); }

// -------------------- Setup & Loop --------------------
void setup() {
    Serial.begin(115200);

    // I/O pins
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_Status, OUTPUT);
    pinMode(LED_WiFi, OUTPUT);
    pinMode(BUTTON_START_PIN, INPUT);
    pinMode(BUTTON_STOP_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    // Servo
    myServo.setPeriodHertz(50);
    myServo.attach(SERVO_PIN, 500, 2400);
    myServo.write(SERVO_DEFAULT_POS);

    // LCD
    lcdManager.initialize();

    // WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        DEBUG_PRINT(".");
        lcdManager.wifiConnecting();
    }
    DEBUG_PRINTLN("WiFi Connected!");

    // Initial statuses
    statusHandler.StatusOnStart(); // Start Power & WiFi statuses 
    blynkManager.BlynkBegin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
    DEBUG_PRINTLN(feedManager.readSchedule(FeedingSchedule, ScheduleCounter));
    statusHandler.FeedStatusReset();
    lcdManager.allSetupDone();
}

void loop() {
    Blynk.run();
    statusHandler.StatusUpdate(); // Update Power, WiFi, and Feed statuses
    status = timeManager.rtcTimer(); // Get Real-Time Clock Timer

    // Button reads
    bool startValue = digitalRead(BUTTON_START_PIN);
    bool stopValue  = digitalRead(BUTTON_STOP_PIN);
    eStop.E_STOP(stopValue == HIGH); // Emergency-Stop

    // Manual feed (activated via physical button)
    feedManager.startFeed(FeedSize::BIG, 5000, false, false);

    if (FeedCounter > 0) { // Start the schedule feeding once there's an item added to the schedules
        feedManager.scheduleFeed(FeedingSchedule, ScheduleCounter, reinterpret_cast<const RtcStatus&>(status));
    }

    feedManager.delayTillReset(FeedingSchedule, ScheduleCounter); // Wait 10 sec before resetting and running for tomorrow
}
