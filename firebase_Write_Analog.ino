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
#define USER_EMAIL "USER"
#define USER_PASSWORD "PASS"

// Define your RTDB URL
#define DATABASE_URL "URL"

// Firebase Data object
FirebaseData fbdo;

// Firebase Authentication object
FirebaseAuth auth;

// Firebase Config object
FirebaseConfig config;

// LED pin (PWM capable)
#define LED_PIN 13  // Use the GPIO pin number where the LED is connected, typically D4 on ESP8266

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
    // Read the Brightness value from Firebase
    if (Firebase.RTDB.getInt(&fbdo, "/led/Brightness")) {
      int brightness = fbdo.intData();  // Get the brightness value
      Serial.print("LED Brightness from Firebase: ");
      Serial.println(brightness);

      // Ensure brightness value is within the range of 0 to 255
      brightness = constrain(brightness, 0, 255);

      // Set LED brightness
      analogWrite(LED_PIN, brightness);  // Set LED brightness (0-255)
    } else {
      // If reading brightness failed, print error message
      Serial.print("Failed to get LED brightness: ");
      Serial.println(fbdo.errorReason());
    }
  }

  // Add a delay to reduce the number of requests sent to Firebase
  delay(1000);
}
