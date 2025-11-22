#include <WiFi.h>
#include <WebServer.h>
/* 
Production ready candidate - Nov 21, 2025
PIN D10 to signal the Sawbird
Complie as XIAO_ESP32C6
External antenna selected - using a 31mm wire
*/
const char* ssid = "YOUR_SSID";
const char* password = "SECRET_PASSWORD";
const int switchPin = D10; // sawbird switch
WebServer server(80);

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
  
  WiFi.begin(ssid, password); // serial print for debugging.  also very handy
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/sawbird/operate", handleOn);
  server.on("/sawbird/50ohm", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}
