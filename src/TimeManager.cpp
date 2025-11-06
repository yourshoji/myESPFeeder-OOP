#include <TimeManager.h>
#include <Arduino.h>

    TimeManager::rtc_src TimeManager::rtcTimer() {
        rtc_src status; // create a local struct variable
        RtcDateTime now = _rtc.GetDateTime();

        status.rtc_hr = now.Hour(); // field struct fields
        status.rtc_min = now.Minute();
        status.rtc_sec = now.Second();
        
        return status;
    }

    // TimeManager::rtc_src status; // declaring (globally)

    String TimeManager::rtc_strOutput(){
        RtcDateTime now = _rtc.GetDateTime();

        String formattedTime = "";
        formattedTime += String(now.Day());
        formattedTime += "/";
        formattedTime += String(now.Month());
        formattedTime += " ";
        formattedTime += String(now.Hour());
        formattedTime += ":";
        formattedTime += String(now.Minute());

        return formattedTime;
    }

    void TimeManager::rtc_firstTimeSetup(){
        RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
        _rtc.SetDateTime(compiled);  
    }