#include <ESP8266WiFi.h>

String apiKey = "APIKEY";  // Enter your Write API key from ThingSpeak

const char *ssid = "WIFIID";  // Replace with your WiFi SSID and WPA2 key
const char *password = "WIFIPASS";
const char* server = "api.thingspeak.com";

WiFiClient client;

//for NodeMCU
const int trigPin = A0;
int value =0;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Serial Begin");
  
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
        
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
}

void loop() 
{
  ldr();
  delay(1000);
  // we have made changes in program to decrease the latency.
  // but it will take minimum 15 second to post data on Thingspeak channel.
    if (client.connect(server,80))
    {
      fwd_to_Thingspeak();
    }
    client.stop();
    Serial.println("Waiting");
    delay(15000);
}

void ldr()
{
 value = analogRead(A0);
}

void fwd_to_Thingspeak()
{
  String postStr = apiKey;
  postStr +="&field1=";
  postStr += String(value);  // ultrasonic data
  postStr += "\r\n\r\n";
          
  client.print("POST /update HTTP/1.1\n");
  client.print("Host: api.thingspeak.com\n");
  client.print("Connection: close\n");
  client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  client.print(postStr.length());
  client.print("\n\n");
  client.print(postStr);
          
  Serial.println("Send data to thingspeak: ");
  Serial.print("Content-Length: ");
  Serial.print(postStr.length());
  Serial.print(" Field-1: ");
  Serial.print(value);  
  Serial.println(" data send");            
}
