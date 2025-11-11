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
#include "TimeManager.h"
#include "LCDManager.h"
#include "Logger.h"
#include "FeedManager.h"
#include "BlynkManager.h"
#include "StatusHandler.h"

// LCD config
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// RTC config
ThreeWire myWire(12, 27, 13);  // IO, SCLK, CE (adjust pins as needed)
RtcDS1302<ThreeWire> Rtc(myWire);

// SERVO config
Servo myServo;

//Instantiation (spawning) of objects, so they truly exist
LCDManager lcdManager(lcd);
TimeManager timeManager(Rtc);
EmergencyStop eStop(BUTTON_STOP_PIN, myServo); // calling a function from another file (setup of E-STOP)
StatusHandler statusHandler;
FeedManager feedManager(
    myServo, 
    lcd, 
    Rtc, 
    BUZZER_PIN, 
    LED_Status,
    SERVO_DEFAULT_POS
);

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

// global time status
TimeManager::rtc_src status; // global struct, object of rtc_src

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

  // I/O pins setup
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_Status, OUTPUT);
  pinMode(LED_WiFi, OUTPUT);
  pinMode(BUTTON_START_PIN, INPUT);
  pinMode(BUTTON_STOP_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Servo setup
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2400);
  myServo.write(SERVO_DEFAULT_POS);
  
  // LCD setup
  lcdManager.initialize();      

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DEBUG_PRINT(".");
    
    lcdManager.wifiConnecting();
  }
  
  DEBUG_PRINTLN("WiFi Connected!");

  // statuses (on-start)
  // power status
  statusHandler.StatusOnStart();

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  DEBUG_PRINTLN(feedManager.readFeedTimer(feedingTimes[0]));

  statusHandler.FeedStatusReset();

  DEBUG_PRINTLN("Counter (startup): " + String(feed_arrCounter));
}

void loop() {

  Blynk.run();

  statusHandler.StatusUpdate();

  status = timeManager.rtcTimer(); // update the status (globally)

  startValue = digitalRead(BUTTON_START_PIN);
  stopValue = digitalRead(BUTTON_STOP_PIN);
    
  bool E_STOP_VALUE = (stopValue == HIGH); // true if pressed, false if released
  eStop.E_STOP(E_STOP_VALUE);  

















}

