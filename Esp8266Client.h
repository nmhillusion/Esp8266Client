#ifndef Esp8266Client_H
#define Esp8266Client_H

#include <Arduino.h>

class Esp8266Client{
    public:
        Esp8266Client();
        void setup(int rx, int tx, int baud);
        bool connectAP(String ssid, String password);
        bool checkConnect();
        String sendATCommand(String cmd, int interval, boolean debug);
        String httpGet(String url);
        String httpPost(String url, int numVar, String* body);
};

#endif
