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

#define vertSW1   10
#define horiSW2   12

// Define some steppers and the pins the will use
//AccelStepper stepper1; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
//AccelStepper stepper1(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
//AccelStepper stepper2(AccelStepper::FULL4WIRE, 6, 7, 8, 9);

void setup()
{  
    Serial.begin(9600);
    pinMode(vertSW1, INPUT_PULLUP);
    //pinMode(horiSW2, INPUT_PULLUP);
    // stepper1.setMaxSpeed(2000);
    // stepper1.setAcceleration(1000);
    // stepper1.moveTo(4000);

    // stepper2.setMaxSpeed(1000);
    // stepper2.setAcceleration(800);
    // stepper2.moveTo(350);
}

void loop()
{
    int sensorVal1 = digitalRead(vertSW1);
    //int sensorVal2 = digitalRead(horiSW2);
    Serial.print("Vertical = ");
    Serial.println(sensorVal1);
    //Serial.print("\t");
    //Serial.print("Sw2 = ");
    //Serial.println(sensorVal2);
    delay(1);
    // Change direction at the limits
    // if (stepper1.distanceToGo() == 0) stepper1.moveTo(-stepper1.currentPosition());
    // if (stepper2.distanceToGo() == 0) stepper2.moveTo(-stepper2.currentPosition());
    // stepper1.run();
    // stepper2.run();

}
