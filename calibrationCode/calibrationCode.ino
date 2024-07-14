/*  Motor Homing code using AccelStepper
 
Created by Yvan / https://www.brainy-bits.com/post/homing-stepper-motors-using-the-accelstepper-library#google_vignette
This code is a slightly modified version of the link above
For this setup the horizontal motor has a maximum of 80000 steps from one edge
to another
Vertical has an max of 15900 steps
*/
#include "AccelStepper.h"

// AccelStepper Setup
AccelStepper stepperX(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper stepperZ(AccelStepper::FULL4WIRE, 6, 7, 8, 9);
// Define the Pins used
#define xRS 10  // Pin 12 connected to Home Switch (MicroSwitch)
#define xLS 11
#define zTS 12  // Pin 12 connected to Home Switch (MicroSwitch)
#define zBS 13
#define maxSpd 2000
#define normSpd 1500
#define acclSpd 1000
#define STEP_PER_MM 320
#define MM_PER_STEP 0.003125

void setup() {
  Serial.begin(9600);
  pinMode(xRS, INPUT_PULLUP);
  pinMode(xLS, INPUT_PULLUP);
  pinMode(zBS, INPUT_PULLUP);
  pinMode(zTS, INPUT_PULLUP);

  //  Set Max Speed and Acceleration of each Steppers at startup for homing
  stepperX.setMaxSpeed(maxSpd);       // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(acclSpd);  // Set Acceleration of Stepper
  stepperX.setSpeed(normSpd);
  stepperZ.setMaxSpeed(maxSpd);       // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperZ.setAcceleration(acclSpd);  // Set Acceleration of Stepper
  stepperZ.setSpeed(normSpd);
}


void loop() {
  // xTestHoming();
  zTestHoming();
}

void xTestHoming() {
  // Start Homing procedure of Stepper Motor at startup
  while (digitalRead(xRS) == 0) {  // Make the Stepper move CW until the switch is activated
    stepperX.move(1600);           // Set the position to move to
    stepperX.runSpeed();           // Start moving the stepper
  }
  // Serial.println(stepperX.currentPosition());
  stepperX.setCurrentPosition(0);
  stepperX.runToNewPosition(-800);

  // Start Homing procedure of Stepper Motor at startup
  while (digitalRead(xLS) == 0) {  // Make the Stepper move CW until the switch is activated
    stepperX.move(-1600);                   // Set the position to move to
    stepperX.runSpeed();                   // Start moving the stepper
  }
  stepperX.setCurrentPosition(0);
  stepperX.runToNewPosition(800);
}

void zTestHoming() {
  // Start Homing procedure of Stepper Motor at startup
  while (digitalRead(zTS) == 1) {  // Make the Stepper move CW until the switch is activated
    stepperZ.move(-1600);          // Set the position to move to
    stepperZ.runSpeed();           // Start moving the stepper
  }
  stepperZ.setCurrentPosition(0);
  stepperZ.runToNewPosition(800);

  // Start Homing procedure of Stepper Motor at startup
  while (digitalRead(zBS) == 1) {  // Make the Stepper move CW until the switch is activated
    stepperZ.move(1600);           // Set the position to move to
    stepperZ.runSpeed();           // Start moving the stepper
  }
  stepperZ.setCurrentPosition(0);
  stepperZ.runToNewPosition(-800);
}