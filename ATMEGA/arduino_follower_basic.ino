/*
  Arduino Slave for Raspberry Pi Master
  i2c_slave_ard.ino
  Connects to Raspberry Pi via I2C
  
  DroneBot Workshop 2019
  https://dronebotworkshop.com
*/

// Include the Wire library for I2C
#include <Wire.h>

// LED on pin 13
const int ledPin = 13; 

//ultrasonic pins:
const int trigPin = 11;
const int echoPin = 12;
const int leftPin = 5;           //green LED represents left vibration module
const int rightPin = 6; //right vibration module


float distance = 0;

//byte data_to_send_to_pi = 0x06;
int  data_to_send_to_pi = 0;

void setup() {
  // Join I2C bus as slave with address 8
  Wire.begin(0x8);
  
  // Call receiveEvent when data received                
  Wire.onReceive(receiveEvent);

  Wire.onRequest(sendEvent);
  
  // Setup pin 13 as output and turn LED off
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  //ultrasonic setup:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(leftPin, OUTPUT);
  digitalWrite(leftPin, LOW);
  pinMode(rightPin, OUTPUT);
  digitalWrite(rightPin, LOW);
}

// Function that executes whenever data is received from master
void receiveEvent(int howMany) {
  digitalWrite(leftPin, LOW);
  digitalWrite(rightPin, LOW);
  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    // if((int)c == 1){ // for type conversion testing
    //   Serial.println("received -1");
    // }
    // Serial.print("without conversion: ");
    // Serial.println(c);
    Serial.println((int)c);
    digitalWrite(ledPin, c);
    if(((int)c ==1)||((int)c==3)){ //received LEFT
      //turn left LED on 
      digitalWrite(leftPin, 1);
    }
    if(((int)c==2)||((int)c==3)){ //received RIGHT
      //turn right LED on
      digitalWrite(rightPin, 1);
    }

  }
  // digitalWrite(leftPin, LOW);
  // digitalWrite(rightPin, LOW);

}

void sendEvent(){
  Wire.write(data_to_send_to_pi);
  digitalWrite(leftPin, LOW);
  digitalWrite(rightPin, LOW);
}
void loop() {
  //ultrasonic get data:
  distance = getDistance();
  if (distance <=10){
    data_to_send_to_pi = 1; //1 IS CLOSE
  }
  else{
    data_to_send_to_pi = 2; //2 IS FAR
  }
  //data_to_send_to_pi = distance;
  // Serial.print("distance: ");
  // Serial.print(distance);
  // Serial.println(" in");

  delay(100);
}

//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance()
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  return calculatedDistance;              //send back the distance that was calculated
}
