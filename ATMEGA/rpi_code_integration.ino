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

  
  if(distance <= 30) {
    //Close up vibration
    if((i2c_data_byte == 1 || i2c_data_byte/10 == 1)) {
      digitalWrite(vibration_left, 1);
      digitalWrite(vibration_right, 0);
    }
    else if(i2c_data_byte == 2 || i2c_data_byte/10 == 2) {
      digitalWrite(vibration_left, 1);
      digitalWrite(vibration_right, 1);
    }
    else if(i2c_data_byte == 3 || i2c_data_byte/10 == 3) {
      digitalWrite(vibration_left, 0);
      digitalWrite(vibration_right, 1);
    }
    else {
      digitalWrite(vibration_left, 0);
      digitalWrite(vibration_right, 0);
    }
  }
  else {
    //Far away vibration
    if((i2c_data_byte/10) > 0) {
      if(vibes_counter == 0) {
        digitalWrite(vibration_left, 1);
        digitalWrite(vibration_right, 1);
        vibes_counter++;
      }
      else if((i2c_data_byte/10) > 0 && vibes_counter < 5) {
        digitalWrite(vibration_left, 1);
        digitalWrite(vibration_right, 1);
        vibes_counter++;
      }
      else if((i2c_data_byte/10) > 0 && vibes_counter >= 5 && vibes_counter < 50) {
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
  //    1 - vibrate left if < 50cm
  //    2 - vibrate left and right if < 50cm
  //    3 - vibrate right if < 50cm
  //   10 - vibrate lightly if person detected in camera
  //   20 - vibrate lightly if person detected in camera
  //   30 - vibrate lightly if person detected in camera
  
  delay(100);


  
}

void receiveData(int bytecount)
{
  for (int i = 0; i < bytecount; i++) {
    i2c_data_byte = Wire.read();
  }
  // Serial.println(i2c_data_byte);
}

// void sendData()
// {
//   Wire.write(i2c_data_byte);
// }