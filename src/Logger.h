#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include "credentials.h" // for webApp variable

class Logger {
    private:
        String _webApp = String(webApp);

    public:
        void sheetLogger(String val1, String val2, String val3);
    };