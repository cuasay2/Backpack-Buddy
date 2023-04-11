#include <Wire.h>

#define SLAVE_ADDRESS 0x08

byte data_to_echo = 0;
const int trigPin = 12; //PD6
const int echoPin = 6; //PD7
const int LED_left = 5;
const int heartbeat_LED = 18; //PF7
const int vibration_right = 10; //PB6
// defines variables
long duration;
int distance;
int heartbeat_counter; 
int temp = 0;

void setup() 
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(heartbeat_LED, OUTPUT);
  pinMode(LED_left, OUTPUT);
  pinMode(vibration_right, OUTPUT);
  Serial.begin(9600); // Starts the serial communication
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  //Wire.onRequest(sendData);
}

void loop() { 

  //Heartbeat
  if(heartbeat_counter >= 100000 && temp == 0) {
    heartbeat_counter = 0;
    temp = 1;
    digitalWrite(heartbeat_LED, HIGH);
  }
  else if(heartbeat_counter >= 100000 && temp == 1) {
    heartbeat_counter = 0;
    temp = 0;
    digitalWrite(heartbeat_LED, LOW);
  }
  heartbeat_counter = heartbeat_counter + 1;

  //Ultrasonic Sensor with RPI
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
  Serial.println(data_to_echo);
  if(data_to_echo == 1 && distance <= 30){
    //digitalWrite(LED_left, 1);
    digitalWrite(vibration_right, 0);
  }
  else if(data_to_echo == 2 && distance <= 30){
    //digitalWrite(LED_left, 1);
    digitalWrite(vibration_right, 1);
  }
  else if(data_to_echo == 3 && distance <= 30){
    //digitalWrite(LED_left, 0);
    digitalWrite(vibration_right, 1);
  }
  else {
    digitalWrite(vibration_right, 0);
    //digitalWrite(LED_left, 0);
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