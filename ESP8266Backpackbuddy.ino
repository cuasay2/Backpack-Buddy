#include <ESP8266WiFi.h> 
#include "SerialTransfer.h"
// install SafeString library from the library manager for PinFlasher
#include <PinFlasher.h>

SerialTransfer myTransfer;

const char* ssid     = "Jeric's iPhone";
const char* password = "poopoopeepee";

const char* resource = "https://maker.ifttt.com/trigger/backpackbuddy/with/key/UjwdT7N64coGcu_B1RmSp";
const char* server = "maker.ifttt.com";

int ledState = LOW;

//heartbeat initialization
int flasher_pin = 2; // GPIO2 for ESP-01S
PinFlasher flasher(flasher_pin, true); // true for invert output logic,
// default logic is HIGH for on, inverted logic is LOW for on
// ESP pins are active LOW so should invert so flasher.setOnOff(PIN_ON) turns led on.

void setup() {
Serial.begin(115200);
  flasher.setOnOff(1000); // on for 1sec then off for 1sec; heartbeat setup

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  myTransfer.begin(Serial);
}

void loop() 
{
  flasher.update(); // should call this often, atleast every loop(); basically heartbeat loop

    
  if(myTransfer.available())
  {
    int status;  
    myTransfer.rxObj(status, sizeof(status));
    if (status==1)
    {
        WiFiClient client;
        client.print(String("GET ") + resource + 
                        " HTTP/1.1\r\n" +
                        "Host: " + server + "\r\n" + 
                        "Connection: close\r\n\r\n");
                    
        int timeout = 5 * 10; // 5 seconds             
        while(!!!client.available() && (timeout-- > 0))
        {
        delay(100);
        }
        while(client.available())
        {
        Serial.write(client.read());
        }
        client.stop();
        delay(10000);
    }      
  } 
}  
