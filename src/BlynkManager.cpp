#include "BlynkManager.h"

    void BlynkManager::BlynkSetup() {
        Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
    }

    