#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// Provide the RTDB payload printing info and other helper functions.
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Define your Wi-Fi credentials
#define WIFI_SSID "WIFI"
#define WIFI_PASSWORD "PASS"

// Define your Firebase project API Key
#define API_KEY "API"

// Define Authorized Username and Corresponding Password
#define USER_EMAIL "GMAIL.com"
#define USER_PASSWORD "PASS"

// Define your RTDB URL
#define DATABASE_URL "URL"

// Firebase Data object
FirebaseData fbdo;

// Firebase Authentication object
FirebaseAuth auth;

// Firebase Config object
FirebaseConfig config;

// LDR pin
#define LDR_PIN A0 // Analog pin connected to the LDR

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set up LDR pin as input (usually done by default for analog pins)
  pinMode(LDR_PIN, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  // Assign Firebase project API Key
  config.api_key = API_KEY;

  // Assign the RTDB URL
  config.database_url = DATABASE_URL;

  // Assign the user sign-in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Initialize Firebase
  Firebase.begin(&config, &auth);

  // Set the maximum retry of token generation
  Firebase.reconnectWiFi(true);

  // Verify Firebase connection
  if (Firebase.ready()) {
    Serial.println("Connected to Firebase");
  } else {
    Serial.println("Failed to connect to Firebase");
  }
}

void loop() {
  // Read the LDR value
  int ldrValue = analogRead(LDR_PIN);  // Read the analog value from the LDR
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Send the LDR value to Firebase
  if (Firebase.RTDB.setInt(&fbdo, "/ldr/value", ldrValue)) {
    Serial.println("LDR value sent to Firebase successfully");
  } else {
    Serial.print("Failed to send LDR value: ");
    Serial.println(fbdo.errorReason());
  }

  // Add a delay before the next reading
  delay(1000);
}
