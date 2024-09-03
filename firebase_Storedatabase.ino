#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Firebase_ESP_Client.h>

// Firebase includes and configurations
#include "addons/TokenHelper.h" 
#include "addons/RTDBHelper.h"

// Define your Wi-Fi credentials
#define WIFI_SSID "WIFI"
#define WIFI_PASSWORD "PASS"

// Define your Firebase project API Key
#define API_KEY "API"

// Define Authorized Username and Corresponding Password
#define USER_EMAIL "GMAIL"
#define USER_PASSWORD "PASS"

// Define your RTDB URL
#define DATABASE_URL "URL"

// Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// NTP Client configuration
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // Time zone offset in seconds for IST (UTC+5:30)

#define LDR_PIN A0 // Analog pin connected to the LDR

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  // Initialize NTP Client
  timeClient.begin();

  // Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Update time from NTP server
  timeClient.update();

  // Read LDR value
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Get current time in human-readable format
  String formattedTime = timeClient.getFormattedTime(); // HH:MM:SS
  unsigned long epochTime = timeClient.getEpochTime();  // UNIX timestamp
  
  // Print the time
  Serial.print("Current Time: ");
  Serial.println(formattedTime);

  // Create path for new data entry
  String path = "/ldr_data/" + String(epochTime);

  // Create a FirebaseJson object to store the data
  FirebaseJson json;
  json.add("value", ldrValue);
  json.add("timestamp", epochTime);

  // Send data to Firebase
  if (Firebase.RTDB.setJSON(&fbdo, path, &json)) {
    Serial.println("LDR data sent to Firebase successfully");
  } else {
    Serial.print("Failed to send LDR data: ");
    Serial.println(fbdo.errorReason());
  }

  delay(10000); // Delay for 10 seconds
}
