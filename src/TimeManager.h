#pragma once
#include <Arduino.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

class TimeManager {
    private:
        RtcDS1302<ThreeWire>& _rtc;

    public:
        TimeManager(RtcDS1302<ThreeWire>& rtc): _rtc(rtc){}

        struct rtcStatus {
            int rtc_hr;
            int rtc_min;
            int rtc_sec;
        };

        rtcStatus rtcTimer() {
            rtcStatus status; // create a local struct variable
            RtcDateTime now = _rtc.GetDateTime();

            status.rtc_hr = now.Hour(); // field struct fields
            status.rtc_min = now.Minute();
            status.rtc_sec = now.Second();
            
            return status;
        }

        rtcStatus status; // declaring (globally)

        String rtc_strTimer(){
            RtcDateTime now = _rtc.GetDateTime();

            String formattedTime = "";
            formattedTime += String(now.Day());
            formattedTime += "/";
            formattedTime += String(now.Month());
            // formattedTime += "/";
            // formattedTime += String(now.Year());
            formattedTime += " ";
            formattedTime += String(now.Hour());
            formattedTime += ":";
            formattedTime += String(now.Minute());
            // formattedTime += ":";
            // formattedTime += String(now.Second());

            return formattedTime;
        }

        void firstTimeSetup(){
            RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
            _rtc.SetDateTime(compiled);  
        }
    };