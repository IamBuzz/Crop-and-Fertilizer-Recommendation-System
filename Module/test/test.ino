#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>     // Include ESP8266WiFi library


const char* ssid = "DIVINE GATE";     // Replace with your WiFi network SSID
const char* password = "12345678";    // Replace with your WiFi network password
const char* host = "http://localhost:3000/";   // Replace with the IP address or hostname of your server


void setup() {
  Serial.begin(9600);
  delay(1000);  // Delay to let the serial monitor settle

  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  // Read humidity (%)
  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature) || isnan(humidity)) {
    return;
  }

  // Create JSON string
  char jsonData[100];
  snprintf(jsonData, sizeof(jsonData), "{\"temperature\": %.2f, \"humidity\": %.2f}", temperature, humidity);

  // Send data to server
  sendData(jsonData);
}

void sendData(char* data) {
  WiFiClient client;
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {
    return;
  }

  // Make HTTP POST request to server
  client.println("POST /your_endpoint HTTP/1.1");
  client.println("Host: " + String(host));
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(strlen(data));
  client.println();
  client.println(data);

  delay(10);

  client.stop(); // Close connection
}
