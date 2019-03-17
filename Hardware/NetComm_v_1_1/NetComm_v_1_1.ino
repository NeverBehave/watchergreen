
/*
  Created: October 17, 2011 by Hans Scharler (http://www.iamshadowlord.com)
  Additional Credits:
  Example sketches from Arduino team, Ethernet by Adrian McEwen
  Added dht11 showed dtostrf function by diy_bloke 22/11/2014
*/
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#define DHT11PIN 4  // The Temperature/Humidity sensor
#define LED_BUILTIN 8

int failedCounter = 0;
int photoRead = 0;
DHT dht(DHT11PIN, DHT11);

void setup()
{
  // Start Serial for debugging on the Serial Monitor
  Serial.begin(9600);
  dht.begin();
  attachInterrupt(digitalPinToInterrupt(2), releaseBeacon, CHANGE);
  pinMode(8, OUTPUT);
}
void loop()
{
  // Read value from Analog Input Pin 0
  String analogPin0 = String(analogRead(A0), DEC);

  //------DHT11--------
  char t_buffer[10];
  char h_buffer[10];
  float t = (dht.readTemperature());
  String temp = dtostrf(t, 0, 5, t_buffer);
  //Serial.print(temp);
  //Serial.print(" ");
  float h = (dht.readHumidity());
  String humid = dtostrf(h, 0, 5, h_buffer);
  //Serial.println(humid);
  //------OTHER-------
  photoRead = analogRead(A0);
  Serial.println("temp:" + temp + "humid:" + humid + "light" + photoRead);
  delay(1000);
}
void releaseBeacon() {
  for(;;) {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(400);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(400);
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(400);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(2000);  
  } 
}
