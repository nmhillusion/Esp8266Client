#include "Esp8266Client.h"

Esp8266Client::Esp8266Client() {}

void Esp8266Client::setup(int rx, int tx, int baud) {
	Serial.println("setup for esp8266");

	esp8266 = new SoftwareSerial(rx, tx);
	esp8266->begin(baud);

	sendATCommand("AT+RST", 5000, true); // reset module
	sendATCommand("AT+CWMODE=1", 1000, true); // become client: mode 1
	sendATCommand("AT+CIPMUX=0", 1000, true);
}

String Esp8266Client::sendATCommand(String cmd,
                                    int interval, boolean debug) {
	String result = "";

	cmd += "\r\n";
	esp8266->print(cmd); // send the read character to the esp8266
	Serial.println("command: " + cmd);
	long int time = millis();

	while ((time + interval) > millis()) {
		while (esp8266->available()) {
			//Serial.println(esp8266.available());
			// The esp has data so display its output to the serial window
			char c = esp8266->read(); // read the next character.
			result += c;
		}
		if(result.indexOf("busy") != -1) {
			result = "";
			interval += interval;
		}
	}

	if (debug) {
		Serial.print(result);
	}
	
	esp8266->flush();

	return result;
}

bool Esp8266Client::connectAP(String ssid, String password) {
	sendATCommand("AT+CWSTOPSMART", 2000, true);
	sendATCommand("AT+CWQAP", 2000, true);

	sendATCommand("AT", 1000, true);

	String res = sendATCommand("AT+CWJAP_CUR=\"" + ssid + "\",\"" + password + "\"", 8000, true);
	__ssid = ssid;
	__password = password;

	return res.indexOf("WIFI CONNECTED") != -1;
}

void Esp8266Client::reconnectAP() {
	while (!connectAP(__ssid, __password)) {
		delay(4000);
		if (checkConnect()) break;
	}
}

bool Esp8266Client::checkConnect() {
	String res = sendATCommand("AT+CIPSTATUS", 1000, true);
	for (int i = 0; i < 5; ++i) {
		if (res.indexOf("STATUS:" + String(i)) != -1) return true;
	}
	return false;
}

String Esp8266Client::httpGet(String url, bool autoReconnect) {
	if (!checkConnect()) {
		if(!autoReconnect) {
			return "Error: Not exists connection";
		} else {
			reconnectAP();
		}
	}

	int idxSplit = url.indexOf("/");
	String host = url.substring(0, idxSplit), path = url.substring(idxSplit),
	       cmd = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n";

	sendATCommand("AT+CIPSTART=\"TCP\",\"" + host + "\",80", 2000, true);
	sendATCommand("AT+CIPSEND=" + String(cmd.length() + 2), 2000, true);
	String content = sendATCommand(cmd, 5000, true);

	if (content.indexOf("SEND OK") > -1) {
		if(content.indexOf("200 OK") > -1) {
			int idxContent = content.lastIndexOf("\r\n\r\n");
			content = content.substring(idxContent);
			content.trim();
			content = content.substring(0, content.indexOf("CLOSED"));
		} else {
			content = content.substring(content.indexOf("HTTP/1.1") + 9);
			content = content.substring(content.indexOf("\r\n"));
		}
	}
	sendATCommand("AT", 2000, true);
	return content;
}

String Esp8266Client::httpPost(String url, int numVar, String* body, bool autoReconnect) {
	if (!checkConnect()) {
		if(!autoReconnect) {
			return "Error: Not exists connection";
		} else {
			reconnectAP();
		}
	}

	String bodySend = "";
	for(int i = 0; i < numVar; ++i) {
		bodySend += body[i];

		if(i != numVar - 1) {
			bodySend += "&";
		}
	}

	delete body;

	int idxSplit = url.indexOf("/");
	String host = url.substring(0, idxSplit), path = url.substring(idxSplit),
	       cmd = "POST " + path + " HTTP/1.1\r\nHost: " + host + "\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: "+String(bodySend.length())+"\r\n\r\n"+bodySend+"\r\n";

	sendATCommand("AT+CIPSTART=\"TCP\",\"" + host + "\",80", 2000, true);
	sendATCommand("AT+CIPSEND=" + String(cmd.length() + 2), 2000, true);
	String content = sendATCommand(cmd, 5000, true) + " | " + bodySend;

	if (content.indexOf("SEND OK") > -1) {
		if(content.indexOf("200 OK") > -1) {
			int idxContent = content.lastIndexOf("\r\n\r\n");
			content = content.substring(idxContent);
			content.trim();
		} else {
			content = content.substring(content.indexOf("HTTP/1.1") + 9);
			content = content.substring(content.indexOf("\r\n"));
		}
	}

	sendATCommand("AT", 2000, true);
	return content;
}
