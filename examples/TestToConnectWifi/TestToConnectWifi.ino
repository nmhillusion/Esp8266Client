#include <Esp8266Client.h>
#define DEBUG true

Esp8266Client client;

void setup() {
  Serial.begin(9600);
  Serial.println("TESTING");
  client.setup(8, 9, 9600);

  while (!client.connectAP("Nhatro MinhTam1", "minhtam2017")) {
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
