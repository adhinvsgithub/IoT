
#define BLYNK_TEMPLATE_ID "TMPL3FniDZx9R"
#define BLYNK_TEMPLATE_NAME "Blink LED"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include <Servo.h>

Servo Myservo1;

BLYNK_WRITE(V0)
{
  int Val = param.asInt();
  Serial.println(Val);
  Myservo1.write(Val);
}

void setup()
{
  Serial.begin(115200);
  Myservo1.attach(4,500, 2400);
  delay(100);

  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
}

