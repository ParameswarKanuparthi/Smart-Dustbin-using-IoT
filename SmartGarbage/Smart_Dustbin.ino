#include "ThingSpeak.h"
#define BLYNK_PRINT Serial  
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "Param"; //SSID
char pass[] = "87654321"; // Password



char blynk[]="eTFSKyS4UgTpJ6ZfVmjpgcl0zHC6KSKl";



BlynkTimer timer;
unsigned int move_index = 1; 

const int trigger = 16;
const int echo = 5;
long T;
float distanceCM;
WiFiClient  client;

unsigned long myChannelField = 890224; // Channel ID
const int ChannelField = 1; // Which channel to write data
const char * myWriteAPIKey = "T4OLMM1VZJRXCQJK"; // Your write API Key

void setup()
{
  Serial.begin(115200);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  Blynk.begin(blynk, ssid, pass);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)                                            
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  digitalWrite(trigger, LOW);
  delay(1);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  T = pulseIn(echo, HIGH);
  distanceCM = T * 0.034;
  distanceCM = distanceCM / 2;
  Serial.print("Distance in cm: ");
  Serial.println(distanceCM);
  ThingSpeak.writeField(myChannelField, ChannelField, distanceCM, myWriteAPIKey);
  Blynk.virtualWrite(V5, distanceCM);
  if(distanceCM<30)
  {
    Blynk.notify(String("Dustbin is Full and Distance is: ") + distanceCM);
  }
  if(distanceCM>300)
  {
    Blynk.notify("Dustbin is Empty");
  }
   
  Blynk.run();
  timer.run();
} 
