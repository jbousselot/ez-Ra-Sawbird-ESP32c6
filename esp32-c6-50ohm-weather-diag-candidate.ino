#include <WiFi.h>
#include <WebServer.h>
#include "esp_system.h" 
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "esp_chip_info.h"
#include "esp_flash.h"
/* 
Candidate for the next version. May 2026.
Add weather collection and diagnostics.  
PIN D10 to signal the Sawbird
Complie as XIAO_ESP32C6
External antenna selected - using a 31mm wire
BME280 connected to SDA/SCL for environmental logging
DIAG to show dynamic variables, for fun
*/
Adafruit_BME280 bme; // I2C

const char* chip_info;
const char* ssid = "YOUR_SSID";
const char* password = "SECRET_PASS";
const int switchPin = D10; // sawbird switch
WebServer server(80);
IPAddress staticIP(10, 1, 1, 13); // ESP32 static IP
IPAddress gateway(10, 1, 1, 1);    // IP Address of your network gateway (router)
IPAddress subnet(255, 255, 255, 0);   // Subnet mask
IPAddress primaryDNS(10, 1, 1, 1); // Primary DNS (optional)
IPAddress secondaryDNS(8, 8, 4, 4);   // Secondary DNS (optional)
const char* hostname = "esp32-sawbird";

void handleOn() {
  digitalWrite(switchPin, LOW);
  server.send(200, "text/plain", "SAWBIRD NORMAL"); 
}

void handleOff() {
  digitalWrite(switchPin, HIGH);
  server.send(200, "text/plain", "SAWBIRD BYPASS");
}

void handleWeather() {
    float humidity = bme.readHumidity(); // percentage
    float temp = 1.8 * bme.readTemperature() + 32; //F
    //float temp = bme.readTemperature(); //C
    float pressure = bme.readPressure() / 100.0F; //millibars
    String weather = "T " + String(temp) + " P " + String(pressure) + " H " + String(humidity);
    server.send(200, "text/plain", weather);
}

void printSystemInfo() { //vibecoded
esp_chip_info_t chip_info;
esp_chip_info(&chip_info);
String message = "--- ESP32-C6 Chip Statistics ---\n";
message += "Model: ";
message += (chip_info.model == CHIP_ESP32C6 ? "ESP32-C6\n" : "Unknown\n");

char revBuffer[32];
snprintf(revBuffer, sizeof(revBuffer), "Silicon Revision: v%u.%u\n", chip_info.revision / 100, chip_info.revision % 100);
message += revBuffer;

message += "CPU Cores: " + String(chip_info.cores) + "\n";
message += "Supported Features: ";
  if (chip_info.features & CHIP_FEATURE_WIFI_BGN) message += "Wi-Fi ";
  if (chip_info.features & CHIP_FEATURE_BLE) message += "BLE ";
  if (chip_info.features & CHIP_FEATURE_IEEE802154) message += "802.15.4 (Thread/Zigbee) ";
  if (chip_info.features & CHIP_FEATURE_EMB_FLASH) message += "[Embedded Flash] ";
message += "\n";

char statsBuffer[256];
snprintf(statsBuffer, sizeof(statsBuffer),
  "Flash Chip Size: %u MB\n"
  "Flash Chip Speed: %u MHz\n"
  "Program Heap Size: %u KB\n"
  "Free Heap Size: %u KB\n",
  ESP.getFlashChipSize() / 1024 / 1024,
  ESP.getFlashChipSpeed() / 1000000,
  ESP.getHeapSize() / 1024,
  ESP.getFreeHeap() / 1024
  );
message += statsBuffer;
server.send(200, "text/plain", message);
}

void setup() {
  bool status = bme.begin(0x76);
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
    // Configuring static IP
  if(!WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Failed to configure Static IP");
  } else {
    Serial.println("Static IP configured!");
  }
  
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());  // Print the ESP32 IP address to Serial Monitor

  server.on("/sawbird/operate", handleOn);
  server.on("/sawbird/50ohm", handleOff);
  server.on("/esp32/DIAG", printSystemInfo);
  server.on("/esp32/WEATHER", handleWeather);
  server.begin();
}

void loop() {
  server.handleClient();
}
