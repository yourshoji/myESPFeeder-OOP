#pragma once
#include "config.h"

class StatusHandler {
    private:
        int _POWER_STATUS = 0; // LED_BUILTIN (V0)
        int _PREV_POWER_STATUS = 0;
        int _WIFI_STATUS = 0; // LED_WiFi (V2)
        int _PREV_WIFI_STATUS = 0;
        int _FEED_STATUS = 0; // LED_Status (V1)

    public:
        void StatusOnStart();

        void StatusUpdate();

        void FeedStatusReset();


};
