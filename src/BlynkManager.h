#pragma once
#include <BlynkSimpleEsp32.h>
#include <credentials.h>

class BlynkManager {
    public:
        BlynkManager(
            const char* authToken = BLYNK_AUTH_TOKEN, 
            const char* ssid = WIFI_SSID, 
            const char* pass = WIFI_PASS
        );

        void BlynkSetup();

        
};

