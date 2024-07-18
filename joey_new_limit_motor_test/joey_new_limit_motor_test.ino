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
#define xRS 13  // Pin 12 connected to Home Switch (MicroSwitch)
#define xLS 12
#define zTS 11  // Pin 12 connected to Home Switch (MicroSwitch)
#define zBS 10
#define maxSpd 128000
#define normSpd 32000
#define acclSpd 1000
#define STEP_PER_MM 320
#define MM_PER_STEP 0.003125
#define stopMotors    0

int sensorVal1;
int sensorVal2; // Pin works
int sensorVal3;
int sensorVal4;

int currentVal1;
int currentVal2;
int currentVal3;
int currentVal4;

int prev1;
int prev2;
int prev3;
int prev4;

void setup() {
  Serial.begin(9600);
  pinMode(xRS, INPUT_PULLUP);
  pinMode(xLS, INPUT_PULLUP);
  pinMode(zBS, INPUT_PULLUP);
  pinMode(zTS, INPUT_PULLUP);

  //  Set Max Speed and Acceleration of each Steppers at startup for homing
  stepperX.setMaxSpeed(maxSpd);       // Set Max Speed of Stepper (Slower to get better accuracy)
  //stepperX.setAcceleration(acclSpd);  // Set Acceleration of Stepper
  stepperX.setSpeed(normSpd);
  stepperZ.setMaxSpeed(maxSpd);       // Set Max Speed of Stepper (Slower to get better accuracy)
  //stepperZ.setAcceleration(acclSpd);  // Set Acceleration of Stepper
  stepperZ.setSpeed(normSpd);
  stepperZ.setCurrentPosition(0);

  prev1 = 1;
  prev2 = 1;
  prev3 = 1;
  prev4 = 1;
  //Serial.println("Program begin");
}


void loop() {
  zTestHoming();
  xTestHoming();
  
}

void xTestHoming() {
  //Start Homing procedure of Stepper Motor at startup
  while(1) {  // Make the Stepper move CW until the switch is activated
    stepperX.runSpeed();
  }
  // Serial.println(stepperX.currentPosition());
  stepperX.setCurrentPosition(0);
  stepperX.runToNewPosition(-800);

  // Start Homing procedure of Stepper Motor at startup
  while (failSafe() == 1) {  // Make the Stepper move CW until the switch is activated
    stepperX.runSpeed();
  }
  stepperX.setCurrentPosition(0);
  stepperX.runToNewPosition(800);
}

void zTestHoming() {
  int pos = 0;
  // Start Homing procedure of Stepper Motor at startup
  //stepperZ.setCurrentPosition(pos);
  while (failSafe() == 1) {  // Make the Stepper move CW until the switch is activated
    pos+=1;
    stepperZ.moveTo(pos);
    stepperZ.setSpeed(3000);
    stepperZ.run();
    // pos = pos + 1600;
    // stepperZ.runToNewPosition(pos);
    // stepperZ.move(1600);          // Set the position to move to
    // stepperZ.runSpeed();           // Start moving the stepper
    //Serial.println("In loop");
  }
  //Serial.println("Out of loop1");

  // stepperZ.moveTo(2500);
  // stepperZ.setSpeed(-3000);
  // stepperZ.run();
  // stepperZ.setCurrentPosition(0);
  // stepperZ.runToNewPosition(-2500);
  pos = 0;
  while (failSafe() == 1) {  // Make the Stepper move CW until the switch is activated
    pos+=1;
    stepperZ.moveTo(pos);
    stepperZ.setSpeed(-3000);
    stepperZ.run();
  }
  // Serial.println("Out of loop2");
  // stepperZ.moveTo(2500);
  // stepperZ.setSpeed(3000);
  // stepperZ.run();
  // stepperZ.setCurrentPosition(0);
  // stepperZ.runToNewPosition(2500);
  
}

int failSafe(){
  currentVal1 = digitalRead(13);
  currentVal2 = digitalRead(12); // Pin works
  currentVal3 = digitalRead(11);
  currentVal4 = digitalRead(10);

  sensorVal1 = currentVal1 + prev1;
  sensorVal2 = currentVal2 + prev2;
  sensorVal3 = currentVal3 + prev3;
  sensorVal4 = currentVal4 + prev4;

  prev1 = currentVal1;
  prev2 = currentVal2;
  prev3 = currentVal3;
  prev4 = currentVal4;

  if (sensorVal1 == 0 || sensorVal2 == 0 || sensorVal3 == 0 || sensorVal4 == 0) {
    return stopMotors;
  } 
  else { return 1;}
}