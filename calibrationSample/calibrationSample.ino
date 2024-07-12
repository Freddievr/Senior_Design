/*  Motor Homing code using AccelStepper
 
Created by Yvan / https://www.brainy-bits.com/post/homing-stepper-motors-using-the-accelstepper-library#google_vignette
This code is a slightly modified version of the link above
For this setup the horizontal motor has a maximum of 80000 steps from one edge
to another
Vertical has an max of 15900 steps
*/
#include "AccelStepperWithDistance.h"
#include "AccelStepper.h"

// AccelStepper Setup
AccelStepper stepperX;
// Define the Pins used
#define home_switch 10  // Pin 12 connected to Home Switch (MicroSwitch)

void setup() {
  Serial.begin(9600);  // Start the Serial monitor with speed of 9600 Bauds

  pinMode(home_switch, INPUT_PULLUP);
  //  Set Max Speed and Acceleration of each Steppers at startup for homing
  stepperX.setMaxSpeed(5000);        // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(4000.0);  // Set Acceleration of Stepper
  stepperX.setSpeed(5000);
  // Start Homing procedure of Stepper Motor at startup
  while (digitalRead(home_switch) == 0) {  // Make the Stepper move CW until the switch is activated
    stepperX.move(1600);                   // Set the position to move to
    stepperX.runSpeed();                   // Start moving the stepper
  }


  stepperX.setCurrentPosition(0);
  stepperX.runToNewPosition(-1000);
}

void loop() {
}