/*  Motor Homing code using AccelStepper and the Serial Monitor
 
Created by Yvan / https://Brainy-Bits.com
This code is in the public domain...
You can: copy it, use it, modify it, share it or just plain ignore it!
Thx!

*/

#include "AccelStepper.h"
// Library created by Mike McCauley at http://www.airspayce.com/mikem/arduino/AccelStepper/

// AccelStepper Setup
AccelStepper stepperX(AccelStepper::FULL4WIRE, A0, A1, A2, A3);

// Define the Pins used
#define home_switch 11  // Pin 12 connected to Home Switch (MicroSwitch)

// Stepper Travel Variables
long TravelX;              // Used to store the X value entered in the Serial Monitor
int move_finished = 1;     // Used to check if move is completed
long initial_homing = 1;  // Used to Home Stepper at startup


void setup() {
  Serial.begin(9600);  // Start the Serial monitor with speed of 9600 Bauds

  pinMode(home_switch, INPUT_PULLUP);
  //  Set Max Speed and Acceleration of each Steppers at startup for homing
  stepperX.setMaxSpeed(1000.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(1000.0);  // Set Acceleration of Stepper

  // Start Homing procedure of Stepper Motor at startup
  while (digitalRead(home_switch) == 0) {  // Make the Stepper move CW until the switch is activated
    stepperX.moveTo(initial_homing);  // Set the position to move to
    initial_homing++;                 // Decrease by 1 for next move if needed
    stepperX.run();                   // Start moving the stepper
  }

  stepperX.setCurrentPosition(0);    // Set the current position as zero for now
  stepperX.setMaxSpeed(2000.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(2000.0);  // Set Acceleration of Stepper
  initial_homing = -5;

  while (digitalRead(home_switch) == 1) {  // Make the Stepper move CCW until the switch is deactivated
    stepperX.moveTo(initial_homing);
    stepperX.run();
    initial_homing -= 8;
  }

  stepperX.setCurrentPosition(0);
  stepperX.setMaxSpeed(1000.0);      // Set Max Speed of Stepper (Faster for regular movements)
  stepperX.setAcceleration(1000.0);  // Set Acceleration of Stepper

  // Print out Instructions on the Serial Monitor at Start
  // Serial.println("FINISHED BITCH");
}

void loop() {

 
}