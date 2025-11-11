#include "BlynkManager.h"

void BlynkManager::BlynkBegin(const char* authToken, const char* ssid, const char* pass){
    Blynk.begin(authToken, ssid, pass);
}

void BlynkManager::BlynkSyncVirtualPins() {
    Blynk.syncVirtual(V4);  // Schedule 1 Time
    Blynk.syncVirtual(V5);  // Schedule 2 Time
    Blynk.syncVirtual(V6);  // Schedule Counter
    Blynk.syncVirtual(V13); // Schedule 1 Size
    Blynk.syncVirtual(V14); // Schedule 2 Size
    Blynk.syncVirtual(V15); // Schedule 3 Size
    // syncing data from Blynk Cloud to device
}
