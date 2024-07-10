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
// char command;

// Define some steppers and the pins they will use
AccelStepper stepper_h(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper stepper_v(AccelStepper::FULL4WIRE, 6, 7, 8, 9);

void setup() {  
  Serial.begin(9600);
  pinMode(vertSWPin, INPUT_PULLUP);
  pinMode(horiSWPin, INPUT_PULLUP);
  stepper_h.setMaxSpeed(2000);
  stepper_h.setAcceleration(1000);
  // stepper1.setMaxSpeed(2000);
  // stepper1.setAcceleration(1000);
  // stepper1.moveTo(4000);

  // stepper2.setMaxSpeed(1000);
  // stepper2.setAcceleration(800);
  // stepper2.moveTo(800);
}

void loop() {
  char command;
  // Change direction at the limits
  if (Serial.available() > 0) {
    command = Serial.read();
  }
  //Serial.println(command);
  if (command == "f") {
    Serial.println("forw");
    stepper_h.setSpeed(800);	
    stepper_h.runSpeed();
  }

  if (command == "b") {
    Serial.println("back");
    stepper_h.setSpeed(800);	
    stepper_h.runSpeed();
  }
  delay(100);
  //Serial.println(command);
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
