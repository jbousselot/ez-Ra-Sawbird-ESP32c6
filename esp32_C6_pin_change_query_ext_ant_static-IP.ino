#include <WiFi.h>
#include <WebServer.h>
/* 
Production StaticIP  December 5, 2025
PIN D10 to signal the Sawbird
Complie as XIAO_ESP32C6
External antenna selected - using a small piece of coax with 31mm wire exposed
*/
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const int switchPin = D10; // sawbird switch
WebServer server(80);
IPAddress staticIP(192, 168, 1, 8); 
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 1, 1); //optional
IPAddress secondaryDNS(1, 1, 1, 1); //optional
const char* hostname = "esp32-sawbird";

void handleOn() {
  digitalWrite(switchPin, LOW);
  server.send(200, "text/plain", "SAWBIRD NORMAL"); // feedback for debugging
}

void handleOff() {
  digitalWrite(switchPin, HIGH);
  server.send(200, "text/plain", "SAWBIRD BYPASS"); // feedback for debugging
}

void setup() {
  pinMode(3, OUTPUT); // Two step process to enable the external U.FL port on the C6
  digitalWrite(3, LOW); // activate RF switch control
  delay(100);
  pinMode(14, OUTPUT);   
  digitalWrite(14, HIGH); // select external antenna
  pinMode(switchPin, OUTPUT);
  digitalWrite(switchPin, LOW); //set low as default
  
  Serial.begin(115200); //local debugging mostly
  
  WiFi.setHostname(hostname);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
}
  if(!WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) { //set static IP
    Serial.println("Failed to configure Static IP");
  } else {
    Serial.println("Static IP configured!");
  }
  
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());  // Print the ESP32 IP address to Serial Monitor

  server.on("/sawbird/operate", handleOn);
  server.on("/sawbird/50ohm", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}
