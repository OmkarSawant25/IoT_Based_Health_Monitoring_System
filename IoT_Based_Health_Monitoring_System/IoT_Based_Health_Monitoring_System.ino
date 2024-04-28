#define BLYNK_TEMPLATE_ID "TMPL3a7UbPnDE"
#define BLYNK_TEMPLATE_NAME "DTH"
#define BLYNK_AUTH_TOKEN "2w1p81hzrtdewwKahMKxarC3LAu0kjv7"

#define BLYNK_PRINT Serial
//#include <WiFi.h>
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <DHT.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Omkar";  // type your wifi name
char pass[] = "Omkar123";  // type your wifi password

BlynkTimer timer;


#define DHT11_PIN 2 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHT11_PIN, DHTTYPE);


void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
}
void setup()
{   
  
  Serial.begin(9600);
  

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();
    Serial.println("MLX90614 Test");
  
  // Initialize the sensor
  if(!mlx.begin()) {
    Serial.println("Failed to initialize MLX90614");
    while(1);
  }
  Serial.print("wifi connected");
  timer.setInterval(100L, sendSensor);
 
  }

void loop()
{
    double temp_obj, temp_amb;
  // Get the sensor data
  Serial.print("Reading a measurement...");
  temp_amb = mlx.readAmbientTempC(); //temperature of the sensor itself
  temp_obj = mlx.readObjectTempC();  //temperature of what it's measuring in the 90-degree field of view
  Serial.print("Ambient = "); 
  Serial.print(temp_amb); 
  Serial.print("*C\tObject = "); 
  Serial.print(temp_obj); Serial.println("*C");
   Blynk.virtualWrite(V2, temp_obj);

  // Wait a bit before the next measurement
  delay(500);
  Blynk.run();
  timer.run();
 }
