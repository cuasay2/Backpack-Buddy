#include <Wire.h>

#define SLAVE_ADDRESS 0x08

byte i2c_data_byte = 0;
const int trigPin = 12; //PD6
const int echoPin = 6; //PD7
const int LED_left = 5;
const int heartbeat_LED = A0; //PF7
const int vibration_left = 8; //PB4
const int vibration_right = 10; //PB6

// defines variables
long duration;
int distance;
int heartbeat_counter;
int vibes_counter = 0; 
int temp = 0;
int heartbeatState = 0;
int emergency_data = 0;

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
  Wire.onRequest(sendData);
}

void loop() { 
  emergency_data = 0; //reset emergency stat

  //Heartbeat
  if(heartbeatState == 0){
    heartbeatState = 1;
  }
  else{
    heartbeatState = 0;
  }
  digitalWrite(heartbeat_LED, heartbeatState);

  //Ultrasonic Sensor with RPI
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  //Close up vibration
  if(distance <= 65) {
    digitalWrite(vibration_left, 1);
    digitalWrite(vibration_right, 1);
    //send emergency signal to rpi
    emergency_data = 1; 

  }
  //Far away vibration
  else {
    if((i2c_data_byte/10) > 0) {
      if(vibes_counter < 3) {
        if(i2c_data_byte == 10) {
          digitalWrite(vibration_left, 1);
          digitalWrite(vibration_right, 0);
        }
        else if(i2c_data_byte == 20) {
          digitalWrite(vibration_left, 1);
          digitalWrite(vibration_right, 1);
        }
        else {
          digitalWrite(vibration_left, 0);
          digitalWrite(vibration_right, 1);
        }
        vibes_counter++;
      }

      else if(vibes_counter >= 3 && vibes_counter < 80) {
        digitalWrite(vibration_left, 0);
        digitalWrite(vibration_right, 0);
        vibes_counter++;
      }
      else {
        digitalWrite(vibration_left, 0);
        digitalWrite(vibration_right, 0);
        vibes_counter = 0;
      }
    }
    else {
      digitalWrite(vibration_left, 0);
      digitalWrite(vibration_right, 0);
      vibes_counter = 0;
    }
    
  }
  
  //VIBRATION STATE DECLARATIONS
  //    0 - no vibrations at all
  //   10 - vibrate left if person detected in camera
  //   20 - vibrate both if person detected in camera
  //   30 - vibrate right if person detected in camera
  
  delay(100);


  
}

void receiveData(int bytecount)
{
  for (int i = 0; i < bytecount; i++) {
    i2c_data_byte = Wire.read();
  }
  // Serial.println(i2c_data_byte);
}

void sendData()
{
  Wire.write(emergency_data);
}


