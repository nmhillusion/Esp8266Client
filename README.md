#**ESP8266 Client Library**\n
A library support run 2 methods of HTTP: GET and POST.\n
Example of this library will explain more about how to it working\n

**Basic steps to do**
1. Esp8266Client::setup(RX_PIN, TX_PIN, baudrate)
2. Esp8266Client::connectAP(String ssid, String password)
3. do something else like as Esp8266Client::httpGet(String url, bool autoReconnect)


> @nmhillusion