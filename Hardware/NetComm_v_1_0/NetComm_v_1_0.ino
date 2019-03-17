
/*
  Created: October 17, 2011 by Hans Scharler (http://www.iamshadowlord.com)
  Additional Credits:
  Example sketches from Arduino team, Ethernet by Adrian McEwen
  Added dht11 showed dtostrf function by diy_bloke 22/11/2014
*/
#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#define DHT11PIN 4  // The Temperature/Humidity sensor
#define LED_BUILTIN 8

// Local Network Settings
byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Must be unique on local network
// ThingSpeak Settings
char thingSpeakAddress[] = "184.106.153.149";
String writeAPIKey = "877SYKIVVCWTT6RF";
const int updateThingSpeakInterval = 5 * 1000; // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)
// Variable Setup
long lastConnectionTime = 0;
boolean lastConnected = false;
int failedCounter = 0;
int photoRead = 0;
// Initialize Arduino Ethernet Client
EthernetClient client;
DHT dht(DHT11PIN, DHT11);

void setup()
{
  // Start Serial for debugging on the Serial Monitor
  Serial.begin(9600);
  // Start Ethernet on Arduino
  startEthernet();
  dht.begin();
  attachInterrupt(digitalPinToInterrupt(2), releaseBeacon, CHANGE);
  pinMode(8, OUTPUT);
}
void loop()
{
  // Read value from Analog Input Pin 0
  String analogPin0 = String(analogRead(A0), DEC);
  // Print Update Response to Serial Monitor
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }
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
  // Disconnect from ThingSpeak
  if (!client.connected() && lastConnected)
  {
    Serial.println("...disconnected");
    Serial.println();
    client.stop();
  }
  // Update ThingSpeak
  if (!client.connected() && (millis() - lastConnectionTime > updateThingSpeakInterval))
  {
    Serial.println("temp:" + temp + "humid:" + humid + "light" + photoRead);
    updateThingSpeak("field1=" + temp + "&field2=" + humid + "&field3=" + photoRead);
  }
  // Check if Arduino Ethernet needs to be restarted
  if (failedCounter > 3 ) {
    startEthernet();
  }
  lastConnected = client.connected();
}
void updateThingSpeak(String tsData)
{
  if (client.connect(thingSpeakAddress, 80))
  {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
    lastConnectionTime = millis();
    if (client.connected())
    {
      Serial.println("Connecting to ThingSpeak...");
      Serial.println();
      failedCounter = 0;
    }
    else
    {
      failedCounter++;
      Serial.println("Connection to ThingSpeak failed (" + String(failedCounter, DEC) + ")");
      Serial.println();
    }
  }
  else
  {
    failedCounter++;
    Serial.println("Connection to ThingSpeak Failed (" + String(failedCounter, DEC) + ")");
    Serial.println();
    lastConnectionTime = millis();
  }
}
void startEthernet()
{
  client.stop();
  Serial.println("Connecting Arduino to network...");
  Serial.println();
  delay(1000);
  // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("DHCP Failed, reset Arduino to try again");
    Serial.println();
  }
  else
  {
    Serial.println("Arduino connected to network using DHCP");
    Serial.println();
  }
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
