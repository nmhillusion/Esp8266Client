#include <Esp8266Client.h>
#define DEBUG true

Esp8266Client client;

void setup() {
  Serial.begin(9600);
  Serial.println("TESTING");
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
  */
  client.setup(11, 12, 9600);

  while (!client.connectAP("myWifiName", "myPasswordWifi")) {
    delay(4000);
    if (client.checkConnect()) break;
  }

  Serial.println("result test get: " + client.httpGet("google.com"));

  Serial.println("FINISH TESTING");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readString();
    client.sendATCommand(cmd, 2000, DEBUG);
  }

  Serial.println("\n\n== repost ==\n\n");
  String* body = new String[2] {
    "username=abs",
    "password=123"
  };
  Serial.println("result test POST: " + client.httpPost("nmhillusion.herokuapp.com/login", 2, body));

  delay(10000);
}
