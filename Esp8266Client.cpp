#include <SoftwareSerial.h>
#include <SPI.h>
#include "Esp8266Client.h"

SoftwareSerial* esp8266;

Esp8266Client::Esp8266Client() {}

void Esp8266Client::setup(int rx, int tx, int baud) {
  esp8266 = new SoftwareSerial(rx, tx);
  esp8266->begin(baud);

  sendATCommand("AT+RST", 2000, true); // reset module
  sendATCommand("AT+CWMODE=1", 1000, true); // Chọn chức năng client cho esp
//  sendATCommand("AT+CIPMUX=0", 1000, true);
}

String Esp8266Client::sendATCommand(String cmd,
                                    const int interval, boolean debug) {
  String result = "";

  cmd += "\r\n";
  esp8266->print(cmd); // send the read character to the esp8266
  //  Serial.println("command: " + cmd);
  long int time = millis();

  while ((time + interval) > millis()) {
    while (esp8266->available()) {
      //Serial.println(esp8266.available());
      // The esp has data so display its output to the serial window
      char c = esp8266->read(); // read the next character.
      result += c;
    }
  }

  if (debug) {
    Serial.print(result);
  }

  return result;
}

bool Esp8266Client::connectAP(String ssid, String password) {
  sendATCommand("AT+CWSTOPSMART", 2000, true);
  sendATCommand("AT+CWQAP", 2000, true);
  
  sendATCommand("AT", 1000, true);
  
  String res = sendATCommand("AT+CWJAP_CUR=\"" + ssid + "\",\"" + password + "\"", 8000, true);
  return res.indexOf("WIFI CONNECTED") != -1;
}

bool Esp8266Client::checkConnect() {
  String res = sendATCommand("AT+CIPSTATUS", 1000, true);
  for (int i = 0; i < 5; ++i) {
    if (res.indexOf("STATUS:" + String(i)) != -1) return true;
  }
  return false;
}

String Esp8266Client::httpGet(String url) {
  if (!checkConnect()) return "Error: Not exists connection";

  int idxSplit = url.indexOf("/");
  String host = url.substring(0, idxSplit), path = url.substring(idxSplit),
         cmd = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n";

  sendATCommand("AT+CIPSTART=\"TCP\",\"" + host + "\",80", 2000, true);
  sendATCommand("AT+CIPSEND=" + String(cmd.length() + 2), 2000, true);
  String content = sendATCommand(cmd, 5000, true);

  if (content.indexOf("SEND OK") > -1) {
    int idxContent = content.lastIndexOf("\r\n\r\n");
    content = content.substring(idxContent);
    content.trim();
    content = content.substring(0, content.indexOf("CLOSED"));
  }

  sendATCommand("AT+CIPCLOSE=5", 1000, true);
  
  return content;
}

String Esp8266Client::httpPost(String url, int numVar, String* body) {
  if (!checkConnect()) return "Error: Not exists connection";

  String bodySend = "";
  for(int i = 0; i < numVar; ++i){
    bodySend += body[i];
    
    if(i != numVar - 1){
      bodySend += "&";
    }
  }
  
  int idxSplit = url.indexOf("/");
  String host = url.substring(0, idxSplit), path = url.substring(idxSplit),
         cmd = "POST " + path + " HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: "+String(bodySend.length())+"\r\n\r\n"+bodySend+"\r\nConnection: close\r\n";

  sendATCommand("AT+CIPSTART=\"TCP\",\"" + host + "\",80", 2000, true);
  sendATCommand("AT+CIPSEND=" + String(cmd.length() + 2), 2000, true);
  String content = sendATCommand(cmd, 5000, true) + " | " + bodySend;

//  if (content.indexOf("SEND OK") > -1) {
//    int idxContent = content.lastIndexOf("\r\n\r\n");
//    content = content.substring(idxContent);
//    content.trim();
//    content = content.substring(0, content.indexOf("CLOSED"));
//  }

  sendATCommand("AT+CIPCLOSE=5", 1000, true);
  
  return content;
}
