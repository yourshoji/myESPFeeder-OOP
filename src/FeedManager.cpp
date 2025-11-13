#include "FeedManager.h"
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>
#include "credentials.h"

FeedManager::FeedManager(
    Servo& SERVO, // references
    LiquidCrystal_I2C& LCD, 
    RtcDS1302<ThreeWire>& RTC, 
    int LOCAL_BUZZER_PIN, 
    int LOCAL_LED_PIN,
    int SERVO_DEFAULT_POS): 

    _SERVO(SERVO), 
    _LCD(LCD), 
    _RTC(RTC),  
    _BUZZER_PIN(LOCAL_BUZZER_PIN), 
    _LED_PIN(LOCAL_LED_PIN),
    _SERVO_DEFAULT_POS(SERVO_DEFAULT_POS) {}

int FeedManager::portion(FeedSize size){
    switch(size) {
        case SMALL:
            return 60; // adjust as needed
        case MEDIUM:
            return 120; // adjust as needed
        case BIG:
            return 180; // adjust as needed
        default: return _SERVO_DEFAULT_POS; // default position
    }
}

bool FeedManager::betterDelay(unsigned long duration, unsigned long& delayStart, bool& delaying){
    if (!delaying) {
        delayStart = millis();
        delaying = true;
    }
    if (millis() - delayStart >= duration) {
        delaying = false;
        return true; // delay completed
    }
    return false; // still delaying
}

String FeedManager::rtc_strTimer(){
    RtcDateTime now = _RTC.GetDateTime();
    return String(now.Day()) + "/" + String(now.Month()) + " " + String(now.Hour()) + ":" + String(now.Minute());
}

void FeedManager::startFeed(FeedSize size, int duration, bool isTimer, bool isManual){
    if ((_currentFeedState.active) || _eStop) return;
    
    if (isManual || isTimer) {
        _currentFeedState.active = true;
        _currentFeedState.startTime = millis();
        _currentFeedState.duration = duration;
        _currentFeedState.size = size; 

        digitalWrite(_BUZZER_PIN, HIGH);
        digitalWrite(_LED_PIN, HIGH);
        Blynk.virtualWrite(V1, 1); // Update Feed Status in Blynk

        _LCD.setCursor(0, 0);
        _LCD.print(isTimer ? "TIMED FEEDING" : "FEEDING...");
        _LCD.setCursor(0, 1);
        _LCD.print(rtc_strTimer());

        _SERVO.write(portion(size));
    }
}

void FeedManager::updateFeed(){
    if (!_currentFeedState.active) return;

    if (millis() - _currentFeedState.startTime >= _currentFeedState.duration) {
        digitalWrite(_BUZZER_PIN, LOW);
        digitalWrite(_LED_PIN, LOW);
        Blynk.virtualWrite(V1, 0); // Update Feed Status in Blynk

        _SERVO.write(_SERVO_DEFAULT_POS);
        _LCD.clear();

        _currentFeedState.active = false;
    }
}

void FeedManager::scheduleFeed(FeedTimer* FeedingSchedule, int ScheduleCounter, const RtcStatus& status){
    for (int i = 0; i < ScheduleCounter; i++){
        FeedTimer& t = FeedingSchedule[i];
        if (!t.triggered) {
            int RTC_SEC = status.rtc_hr * 3600 + status.rtc_min * 60 + status.rtc_sec;
            int TIM_SEC = t.hr * 3600 + t.min * 60 + t.sec;
            
            DEBUG_PRINTLN("Checking Schedule " + String(i + 1) + ": RTC_SEC=" + String(RTC_SEC) + ", TIM_SEC=" + String(TIM_SEC));
            
            if (RTC_SEC == TIM_SEC) {
                startFeed(t.size, t.duration, true, false);
                t.triggered = true;

                String msg = "Schedule(" + String(i + 1) + ") is fed!";
                Blynk.logEvent("feeding_on_schedule", msg);
            
                if (i==0) {
                    Blynk.virtualWrite(V7, 1);
                    sheetLogger(WEB_APP, String(i+1), "Yes", "No");
                }
                if (i==1) {
                    Blynk.virtualWrite(V8, 1);
                    sheetLogger(WEB_APP, String(i+1), "Yes", "No");
                }
                if (i==2) {
                    Blynk.virtualWrite(V9, 1);           
                    sheetLogger(WEB_APP, String(i+1), "Yes", "No");
                }
            }
        }
    }
}

void FeedManager::resetSchedule(FeedTimer* FeedingSchedule, int ScheduleCounter){
    for (int i = 0; i < ScheduleCounter; i++){
        FeedingSchedule[i].triggered = false;
    }
    Blynk.virtualWrite(V7, 0);
    Blynk.virtualWrite(V8, 0);
    Blynk.virtualWrite(V9, 0);
}

void FeedManager::delayTillReset(FeedTimer* FeedingSchedule, int ScheduleCounter){
    FeedTimer& t = FeedingSchedule[ScheduleCounter - 1];
    if (t.triggered) {
        if (betterDelay(10000, _delayStartReset, _delayingReset)){
            resetSchedule(FeedingSchedule, ScheduleCounter);
        }
    }
}

// local use only
void FeedManager::sheetLogger(const char* WEB_APP, String val1, String val2, String val3){
    if (WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.setTimeout(5000); // 5 seconds timeout in case of no response
        String url = String(WEB_APP) + "?value1=" + String(val1) + "&value2=" + String(val2) + "&value3=" + String(val3);
        http.begin(url);
        int httpResponseCode = http.GET();  
        if (httpResponseCode > 0) DEBUG_PRINTLN(http.getString());
        else DEBUG_PRINTLN("Error on logging: " + String(httpResponseCode));
        http.end();
    } else DEBUG_PRINTLN("WiFi Disconnected. Logging failed.");
}   

void FeedManager::setEStop(bool state){
    if (state != _prev_eStop){
        _prev_eStop = state;
        if (state) {   
            _SERVO.detach(); // disable servo to stop any movement
            DEBUG_PRINTLN("EMERGENCY STOP ACTIVATED!");
        } else {
            _SERVO.attach(SERVO_PIN); // re-enable servo
            DEBUG_PRINTLN("EMERGENCY STOP DEACTIVATED!");
        }
    }
    _eStop = state;
}

bool FeedManager::getEStop(){
    return _eStop;
}

// DEBUGGER
String FeedManager::readSchedule(FeedTimer* FeedingSchedule, int ScheduleCounter) const{
    String output = "";

    for (int i = 0; i < ScheduleCounter; i++){
        const FeedTimer& t = FeedingSchedule[i];
        output += String(i + 1) + ") ";
        output += "Time: " + String(t.hr) + ":" + String(t.min) + ":" + String(t.sec);
        output += " | Duration: " + String(t.duration);
        output += " | Triggered?: " + String(t.triggered ? "true" : "false");
        output += "\n"; 
    }

    return output;
}

