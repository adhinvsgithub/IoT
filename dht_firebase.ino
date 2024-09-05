#include <Arduino.h>
#include <ESP8266WiFi.h> 
#include <Firebase_ESP_Client.h>
#include "DHT.h"

// Provide the RTDB payload printing info and other helper functions.
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Define your Wi-Fi credentials
#define WIFI_SSID "WIFI"
#define WIFI_PASSWORD "PASS"

// Define your Firebase project API Key
#define API_KEY "API"

// Define Authorized Username and Corresponding Password
#define USER_EMAIL "MAIL"
#define USER_PASSWORD "PASS"

// Define your RTDB URL
#define DATABASE_URL "URL"

#define DHTPIN D2 

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Firebase Data object
FirebaseData fbdo;

// Firebase Authentication object
FirebaseAuth auth;

// Firebase Config object
FirebaseConfig config;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  dht.begin();

  // Set up LDR pin as input (usually done by default for analog pins)
  pinMode(DHTPIN, INPUT);

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

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  // Send the Temperature value to Firebase
  if (Firebase.RTDB.setInt(&fbdo, "/dht/value/temp", t)) {
    Serial.println("Temperature value sent to Firebase successfully");
  } else {
    Serial.print("Failed to send DHT value: ");
    Serial.println(fbdo.errorReason());
  }

  // Send the Humidity value to Firebase
  if (Firebase.RTDB.setInt(&fbdo, "/dht/value/humidity", h)) {
    Serial.println("Temperature value sent to Firebase successfully");
  } else {
    Serial.print("Failed to send DHT value: ");
    Serial.println(fbdo.errorReason());
  }

  // Add a delay before the next reading
  delay(2000);
}
