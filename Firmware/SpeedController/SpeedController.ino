/* 
Proof of concept firmware to generate a SIN waveform and cycle between 50 and 67Hz every 10 seconds
*/

// WiFi credentials
#include <credentials.h>

#include <Arduino.h>
#include <WiFi.h>
#include <PrettyOTA.h>

#include <SPI.h>
#include <Wire.h>

#include <SimpleAD9833.h>

// Define the SPI pins for the AD9833 DDS
const uint8_t SCLK_PIN = 18;
const uint8_t DATA_PIN = 23;
const uint8_t CS_PIN = 5;

AsyncWebServer  server(80); // Server on port 80 (HTTP)
PrettyOTA OTAUpdates;

// Instantiate the SimpleAD9833 object with the chip select (CS) pin
SimpleAD9833 myWaveform(CS_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  pinMode(2, OUTPUT);

  // Initialise SPI communication using the defined pins
  SPI.begin(SCLK_PIN, -1, DATA_PIN);
  Serial.println("Starting SPI");


  // Initialize WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for successful WiFi connection
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
      Serial.println("[WiFi] Connection failed! Rebooting...");
      delay(3000);
      ESP.restart();
  }

    // Print IP address
    Serial.println("PrettyOTA can be accessed at: http://" + WiFi.localIP().toString() + "/update");

    // Initialize PrettyOTA
    OTAUpdates.Begin(&server);

    // Set unique Hardware-ID for your hardware/board
    OTAUpdates.SetHardwareID("ES-SpeedBox-32");

    // Set firmware version to 1.0.0
    OTAUpdates.OverwriteAppVersion("0.0.1");

    // Set current build time and date
    PRETTY_OTA_SET_CURRENT_BUILD_TIME_AND_DATE();

    // Start web server
    server.begin();

  myWaveform.begin();
}

void loop() {
  myWaveform.setWaveform(SINE, 50);
  delay(20000);
  myWaveform.setWaveform(SINE, 67);
  delay(20000);
}