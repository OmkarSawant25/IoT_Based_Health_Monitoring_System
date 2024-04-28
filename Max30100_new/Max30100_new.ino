//SCL    D1
//SDA    D2

#include <Wire.h>
#include <BlynkSimpleEsp8266.h> 
#include <ESP8266WiFi.h>  
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial

BlynkTimer timer;
#define BLYNK_TEMPLATE_ID "TMPL3a7UbPnDE"
#define BLYNK_TEMPLATE_NAME "DTH"
#define BLYNK_AUTH_TOKEN "2w1p81hzrtdewwKahMKxarC3LAu0kjv7"

#define REPORTING_PERIOD_MS     1000

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Omkar";  // type your wifi name
char pass[] = "Omkar123";  // type your wifi password

float BPM, SpO2;

PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected() {
  Serial.println("Beat!");
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  Serial.print("Initializing pulse oximeter..");

  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
    pox.setOnBeatDetectedCallback(onBeatDetected);
  }

  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop() {
  pox.update();

  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("BPM: ");
    Serial.println(BPM);

    Serial.print("SpO2: ");
    Serial.print(SpO2);
    Serial.println("%");

    Serial.println("*");
    Serial.println();

    Blynk.virtualWrite(V3, BPM);
    Blynk.virtualWrite(V4, SpO2);

    tsLastReport = millis();
      Blynk.run();
  timer.run();
  }
}
