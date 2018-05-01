#include "Esp8266Client.h"
#define DEBUG true

//SoftwareSerial esp8266(8, 9); //  10-RX, 11-TX
Esp8266Client client;

char x;
void setup() {
  Serial.begin(9600);
  Serial.println("TESTING");
  client.setup(8, 9, 9600);

  while (!client.connectAP("BON FPT", "kemtuchon19k")) {
    delay(4000);
    if (client.checkConnect()) break;
  }

  Serial.println("result test get: " + client.httpGet("nmhillusion.herokuapp.com/myName/phong"));
  String* body = new String[2] {
    "username=abs",
    "password=123"
  };
  Serial.println("result test get: " + client.httpPost("nmhillusion.herokuapp.com/login", 2, body));
  Serial.println("FINISH TESTING");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readString();
    client.sendATCommand(cmd, 2000, DEBUG);
  }
}
