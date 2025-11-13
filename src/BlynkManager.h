#pragma once
#include <credentials.h>

class BlynkManager {
    public:
        BlynkManager(
            const char* authToken = BLYNK_AUTH_TOKEN, 
            const char* ssid = WIFI_SSID, 
            const char* pass = WIFI_PASS
        );

        void BlynkSyncVirtualPins();

        void BlynkBegin(const char* authToken, const char* ssid, const char* pass);
};

