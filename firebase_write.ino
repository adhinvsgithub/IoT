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
#define API_KEY "APIKEY"

// Define Authorized Username and Corresponding Password
#define USER_EMAIL "MAIL"
#define USER_PASSWORD "PASSWORD"

// Define your RTDB URL
#define DATABASE_URL "URL"

// Firebase Data object
FirebaseData fbdo;

// Firebase Authentication object
FirebaseAuth auth;

// Firebase Config object
FirebaseConfig config;

// LED pin
#define LED_PIN 13  // Use the GPIO pin number where the LED is connected

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Set LED pin as output
  pinMode(LED_PIN, OUTPUT);

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
  // Check if connected to Firebase
  if (Firebase.ready()) {
    // Read the LED state from Firebase
    if (Firebase.RTDB.getInt(&fbdo, "/led/State")) {
      int ledState = fbdo.intData();  // Get the integer value
      Serial.print("LED State from Firebase: ");
      Serial.println(ledState);

      // Set LED state based on Firebase data
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    } else {
      // If reading failed, print error message
      Serial.print("Failed to get LED state: ");
      Serial.println(fbdo.errorReason());
    }
  }

  // Add a delay to reduce the number of requests sent to Firebase
  delay(1000);
}
