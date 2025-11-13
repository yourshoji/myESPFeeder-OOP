#pragma once
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <Arduino.h>

class TimeManager {
    private:
        RtcDS1302<ThreeWire>& _rtc;
        
    public:
        TimeManager(RtcDS1302<ThreeWire>& rtc) : _rtc(rtc){}

        // public for now, i forgot what i coded
        struct rtc_src {
            int rtc_hr;
            int rtc_min;
            int rtc_sec;
        };

        rtc_src rtcTimer();
        String rtc_strOutput();
        void rtc_firstTimeSetup();
    };