#ifndef Esp8266Client_H
#define Esp8266Client_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SPI.h>

/**
     Notice:
      The library has the following known limitations:

    If using multiple software serial ports, only one can receive data at a time.
    Not all pins on the Mega and Mega 2560 support change interrupts,
    so only the following can be used for RX: 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69).

    Not all pins on the Leonardo and Micro support change interrupts,
    so only the following can be used for RX: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
    On Arduino or Genuino 101 the current maximum RX speed is 57600bps
    On Arduino or Genuino 101 RX doesn't work on Pin 13

	Only support 2 HTTP methods: GET and POST
  */

class Esp8266Client {
	public:
		Esp8266Client();
		void setup(int rx, int tx, int baud);
		bool connectAP(String ssid, String password);
		void reconnectAP();
		bool checkConnect();
		String sendATCommand(String cmd, int interval, boolean debug);
		String httpGet(String url, bool autoReconnect = true);
		String httpPost(String url, int numVar, String* body, bool autoReconnect = true);

	private:
		SoftwareSerial* esp8266;
		String __ssid, __password;
};

#endif
