#include "Logger.h"
#include "config.h"
#include <WiFi.h>

void Logger::sheetLogger(String val1, String val2, String val3) {
    if (WiFi.status() == WL_CONNECTED) {    
        HTTPClient http;
        String url = _webApp + "?value1=" + val1 + "&value2=" + val2 + "&value3=" + val3;

        http.begin(url); // Specify the URL
        int httpResponseCode = http.GET(); // Make the request

        if (httpResponseCode > 0) { // Check for the returning code
            DEBUG_PRINT("Logged: ");
            DEBUG_PRINTLN(httpResponseCode);
        } else {
            DEBUG_PRINT("Error code: ");
            DEBUG_PRINTLN(httpResponseCode);
        }
        http.end();
    } else {
    DEBUG_PRINTLN("WiFi not connected, cannot log data.");
    }
}