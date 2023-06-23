//Isaac Masinde
//Techtenum Minds Limited
//23th June 2023
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID       "*****" //Your Wifi Username
#define WLAN_PASS       "**********" //Your Wifi Password
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "*****" //Your Adafruit IO Username
#define AIO_KEY         "*********" //Your Adafruit IO Password
WiFiClient client;
//Setup Library
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Potentiometer = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Potentiometer");

void MQTT_connect();
void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

}

void loop() {
  MQTT_connect();
  int t  = map(analogRead(A0), 0, 1023, 1, 600);
  if (! Potentiometer.publish(t)) {
    Serial.println(F("Potentimeter Failed"));
  } else {
    Serial.println(F("Potentimeter  OK!"));
  }
  delay(3000);
}

void MQTT_connect() 
{
  int8_t ret;
  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
