#include <Wire.h>

#define SLAVE_ADDRESS 0x08

byte data_to_echo = 0;

void setup() 
{
  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);
  //Wire.onRequest(sendData);
}

void loop() { }

void receiveData(int bytecount)
{
  for (int i = 0; i < bytecount; i++) {
    data_to_echo = Wire.read();
  }
  Serial.println(data_to_echo);
}

// void sendData()
// {
//   Wire.write(data_to_echo);
// }