#include "FeedManager.h"

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

int FeedManager::portion(const String& size){
    if (size == "small"){
        return 60; // adjust as needed
    }
    else if (size == "medium"){
        return 120; // adjust as needed
    }
    else if (size == "big"){
        return 180; // adjust as needed
    }
    else {
        return _SERVO_DEFAULT_POS; // default position
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

void FeedManager::startFeed(String size, int duration, bool isTimer, bool isManual){
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

void FeedManager::scheduleFeed(FeedTimer* feedingTimes, int arrLength, const RtcStatus& status){
    for (int i = 0; i < arrLength; i++){
        FeedTimer& t = feedingTimes[i];
        if (!t.triggered) {
            int RTC_SEC = status.rtc_hr * 3600 + status.rtc_min * 60 + status.rtc_sec;
            int TIM_SEC = t.hr * 3600 + t.min * 60 + t.sec;
            if (RTC_SEC == TIM_SEC) {
                startFeed(t.size, t.duration, true, false);
                t.triggered = true;

                String msg = "Schedule(" + String(i + 1) + ") is fed!";
                Blynk.logEvent("feeding_on_schedule", msg);
            
                if (i==0) Blynk.virtualWrite(V7, 1);
                if (i==1) Blynk.virtualWrite(V8, 1);
                if (i==2) Blynk.virtualWrite(V9, 1);            
            }
        }
    }
}

void FeedManager::resetSchedule(FeedTimer* feedingTimes, int arrLength){
    for (int i = 0; i < arrLength; i++){
        feedingTimes[i].triggered = false;
    }
    Blynk.virtualWrite(V7, 0);
    Blynk.virtualWrite(V8, 0);
    Blynk.virtualWrite(V9, 0);
}

void FeedManager::delayTillReset(FeedTimer* feedingTimes, int arrLength){
    FeedTimer& t = feedingTimes[arrLength - 1];
    if (t.triggered) {
        if (betterDelay(10000, _delayStartReset, _delayingReset)){
            resetSchedule(feedingTimes, arrLength);
        }
    }
}

// locally use only
void FeedManager::sheetLogger(const char* webApp, String val1, String val2, String val3){
    if (WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        String url = String(webApp) + "?value1=" + String(val1) + "&value2=" + String(val2) + "&value3=" + String(val3);
        http.begin(url);
        int httpResponseCode = http.GET();  
        if (httpResponseCode > 0) DEBUG_PRINTLN(http.getString());
        else DEBUG_PRINTLN("Error on logging: " + String(httpResponseCode));
        http.end();
    } else Serial.println("WiFi Disconnected. Logging failed.");
}   

void FeedManager::setEStop(bool state){
    if (state != _prev_eStop){
        _prev_eStop = state;
        if (state) {   
            _SERVO.detach(); // disable servo to stop any movement
            Serial.println("EMERGENCY STOP ACTIVATED!");
        } else {
            _SERVO.attach(SERVO_PIN); // re-enable servo
            Serial.println("EMERGENCY STOP DEACTIVATED!");
        }
    }
    _eStop = state;
}

bool FeedManager::getEStop(){
    return _eStop;
}

String FeedManager::readFeedTimer(const FeedTimer& t){
    String output = 
    "Time: " + String(t.hr) + ":" + String(t.min) + ":" + String(t.sec)
    + " Duration: " + String(t.duration)
    + " | Is Triggered?: " + String(t.triggered ? "true" : "false"); 

    return output;
}

