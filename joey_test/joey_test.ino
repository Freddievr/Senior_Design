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
#define increment   50

int numSteps = 50;

// Define some steppers and the pins the will use
AccelStepper stepper_h(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper stepper_v(AccelStepper::FULL4WIRE, 6, 7, 8, 9);

void setup() {  
  Serial.begin(9600);
  pinMode(vertSWPin, INPUT_PULLUP);
  pinMode(horiSWPin, INPUT_PULLUP);
  stepper_h.setMaxSpeed(2000);
  stepper_h.setAcceleration(1000);
  stepper_h.moveTo(numSteps);

  stepper_v.setMaxSpeed(2000);
  stepper_v.setAcceleration(1000);
  stepper_v.moveTo(numSteps);
}

void loop() {
  
  stepper_v.run();
  if (stepper_v.distanceToGo() == 0) {
    numSteps += increment;
  }
  stepper_v.moveTo(numSteps);
  //delay(2000);

   stepper_h.run();
  if (stepper_h.distanceToGo() == 0) {
    numSteps += increment;
  }
  stepper_h.moveTo(numSteps);
}
