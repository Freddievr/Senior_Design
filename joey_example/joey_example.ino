// MultiStepper.pde
// -*- mode: C++ -*-
//
// Shows how to multiple simultaneous steppers
// Runs one stepper forwards and backwards, accelerating and decelerating
// at the limits. Runs other steppers at the same time
//
// Copyright (C) 2009 Mike McCauley
// $Id: MultiStepper.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

#define vertSWPin   10
#define horiSWPin   11
#define stepperSpeed 1000

String command;
int distance = 1500;
int initialPosition = 0;
// Define some steppers and the pins the will use
AccelStepper stepperH(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper stepperV(AccelStepper::FULL4WIRE, 6, 7, 8, 9);

void setup() {  
  Serial.begin(9600);
  pinMode(vertSWPin, INPUT_PULLUP);
  pinMode(horiSWPin, INPUT_PULLUP);
  stepperH.setMaxSpeed(4000);
  stepperH.setAcceleration(1000);
  stepperH.setSpeed(2000);	

  stepperV.setMaxSpeed(2000);
  stepperV.setAcceleration(1000);
  stepperV.setSpeed(1000);
  // stepper1.setMaxSpeed(2000);
  // stepper1.setAcceleration(1000);
  // stepper1.moveTo(4000);

  // stepper2.setMaxSpeed(1000);
  // stepper2.setAcceleration(800);
  // stepper2.moveTo(800);
  stepperH.setCurrentPosition(0);
  stepperV.setCurrentPosition(0);
}

void loop() {
  stepperH.moveTo(100);
  stepperH.run();
  
  // Change direction at the limits
  // Serial.print("Status: ");
  // Serial.println(Serial.available());
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    // Serial.println("Moved into if");
  }
  if (command == "f") {
    // Serial.println(initialPosition);
    // Serial.println("forw");
  //   stepperH.setSpeed(stepperSpeed);	
  //   // stepperH.runSpeed();
    stepperH.moveTo(distance);
  //   // initialPosition++;
    stepperH.run();
    stepperV.moveTo(distance);
    stepperV.run();
  }

  if (command == "b") {
    // Serial.println(command);
    // Serial.println("back");
    // stepperH.setSpeed(stepperSpeed);	
    // stepperH.runSpeed();
    // while(stepperH.distanceToGo() != 0) {
      stepperH.moveTo(-distance);
      // // initialPosition--;
      stepperH.run();

      stepperV.moveTo(-distance);
      stepperV.run();
    // }
  }

  if (command == "s") {
    // Serial.println(command);
    // Serial.println("back");
    // stepperH.setSpeed(stepperSpeed);
    // stepperH.runSpeed();
    stepperH.moveTo(0);
    stepperH.run();
    stepperV.moveTo(0);
    stepperV.run();
  }


  // while (stepperH.distanceToGo() == 0) {
  //   stepperH.setMaxSpeed(1000);
  //   stepperH.moveTo(0);
  // }
  // delay(5);
  // Serial.println(command);
  //delay(10);
  //stepper1.moveTo(400);
  //stepper1.run();
  // while (stepper1.distanceToGo() == 0) {
  //   stepper1.setMaxSpeed(1000);
  //   stepper1.moveTo(0);
  //   stepper2.moveTo(0);
  // }
  // while (failSafe(1) == 0 && failSafe(2) == 0) {
  //   if (stepper1.distanceToGo() == 0) stepper1.moveTo(-stepper1.currentPosition());
  //   if (stepper2.distanceToGo() == 0) stepper2.moveTo(-stepper2.currentPosition());
  //   stepper1.run();
  //   stepper2.run();
  //   //Serial.println("here");
  // }

  // while(1){
  //   stepper1.setMaxSpeed(1000);
  //   stepper1.setAcceleration(800);
  //   stepper1.moveTo(0);
  //   stepper2.setMaxSpeed(1000);
  //   stepper2.setAcceleration(800);
  //   stepper2.moveTo(0);
  // }
}

int failSafe(int switchNum) {
  //delay(1);
  int sensorVal1 = digitalRead(vertSWPin);
  int sensorVal2 = digitalRead(horiSWPin);

  // Serial.print("Vertical = ");
  // Serial.print(sensorVal1);
  // Serial.print("\t");
  // Serial.print("Horizontal = ");
  // Serial.println(sensorVal2);

  if(switchNum == 1) return sensorVal1;
  if(switchNum == 2) return sensorVal2;

}
