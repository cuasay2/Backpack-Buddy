#include <Wire.h>

#define SLAVE_ADDRESS 0x08

byte data_to_echo = 0;
const int trigPin = 11;
const int echoPin = 12;
const int LED_left = 5;
const int LED_right = 6;
// defines variables
long duration;
int distance;

void setup() 
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(LED_left, OUTPUT);
  pinMode(LED_right, OUTPUT);
  Serial.begin(9600); // Starts the serial communication
  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);
  //Wire.onRequest(sendData);
}

void loop() { 
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance);
  //data_to_echo = Wire.read();
  Serial.println(distance);
  if(data_to_echo == 1 && distance <= 30){
    digitalWrite(LED_left, 1);
    digitalWrite(LED_right, 0);
  }
  else if(data_to_echo == 2 && distance <= 30){
    digitalWrite(LED_left, 1);
    digitalWrite(LED_right, 1);
  }
  else if(data_to_echo == 3 && distance <= 30){
    digitalWrite(LED_left, 0);
    digitalWrite(LED_right, 1);
  }
  else {
    digitalWrite(LED_right, 0);
    digitalWrite(LED_left, 0);
  }
  
  delay(100);


  
}

void receiveData(int bytecount)
{
  for (int i = 0; i < bytecount; i++) {
    data_to_echo = Wire.read();
  }
  // Serial.println(data_to_echo);
}

// void sendData()
// {
//   Wire.write(data_to_echo);
// }