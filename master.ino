//James Rossano
//4-13-2023
//Arduino Master Code

#include <Wire.h>
#define SLAVE_ADDR 9
#define BUTTON_1 4
#define BUTTON_2 6
 
void setup() {
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  Wire.begin();
  Serial.begin(9600);
}
 
void loop() {
  
  if(digitalRead(BUTTON_1) == HIGH) {
    Serial.println("playter 1 button pressed");
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(1); //writes that player 1 pressed button to slave
    Wire.endTransmission();
  }
  
  if(digitalRead(BUTTON_2) == HIGH) {
    Serial.println("playter 2 button pressed");
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(2); //writes that player 2 pressed button to slave
    Wire.endTransmission();
  }
  
}
