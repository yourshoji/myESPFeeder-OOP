#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

using namespace std;

#include "EmergencyStop.h"
#include "config.h"
#include "credentials.h"

// LCD config
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// RTC config
ThreeWire myWire(12, 27, 13);  // IO, SCLK, CE (adjust pins as needed)
RtcDS1302<ThreeWire> Rtc(myWire);

// For betterDelay function
unsigned long delayStartFeed = 0;
bool delayingFeed = false;

unsigned long delayStartBlink = 0;
bool delayingBlink = false;

unsigned long delayStartReset = 0;
bool delayingReset = false;

unsigned long delayStartLog = 0;
bool delayingLog = false;

// Triggers
// bool eStop = false;
// bool prev_eStop = false;
bool actionStarted = false;
bool timerTriggered = false;
bool isTimer = false; // triggers with onTimer(), only when the clock hits

int Power_Status = 0; // LED_BUILTIN (V0)
int prev_Power_Status = 0;
int Feed_Status = 0; // LED_Status (V1)
int WiFi_Status = 0; // LED_WiFi (V2)
int prev_WiFi_Status = 0;

// reading the values from Start & E-Stop buttons
bool startValue;
bool stopValue;

Servo myServo;

// create an EmergencyStop object with its constructor
EmergencyStop eStop(buttonStopPin, myServo);

// struct; has all its members public by default
// class; has all its members private by default
struct feedTimer {
  int hr;
  int min;
  int sec;
  String size;
  int duration;
  bool triggered;
};

feedTimer feedingTimes[3] = {
  // {14, 43, 0, "small", 3000, false},
  // {14, 45, 0, "medium", 5000, false},
  // {14, 47, 0, "big", 7000, false}
};

// sizeof() = total size in byte (3 elements * 2 bytes each = 6 bytes)
// total of 6 bytes divided by first byte = 2; ans = 3;
int arrLength = sizeof(feedingTimes) / sizeof(feedingTimes[0]);
int lastIndex = arrLength - 1;

int feed_arrCounter = 0;

struct rtcStatus {
  int rtc_hr;
  int rtc_min;
  int rtc_sec;
};

rtcStatus rtcTimer() {
  rtcStatus status; // create a local struct variable
  RtcDateTime now = Rtc.GetDateTime();

  status.rtc_hr = now.Hour(); // field struct fields
  status.rtc_min = now.Minute();
  status.rtc_sec = now.Second();
  
  return status;
}

rtcStatus status; // declaring (globally)

struct FeedState {
  bool active = false;
  unsigned long startTime = 0;
  int duration = 0;
  String size = "";
};

FeedState currentFeed;

BLYNK_CONNECTED(){
  Blynk.syncVirtual(V4, V5, V6, V13, V14, V15);
}



void setup() {
  
  Serial.begin(115200);

}

void loop() {
  
}

