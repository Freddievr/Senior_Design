#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <string.h>
#include <Stream.h>

// Defined Variables
#define MAX_MOTOR_SPEED 100

// Global Variables
String streamData;
String parsedData;

// Initialize Stepper motors
// Function nomenclature motorName(# of Required Input pin(STEP and DIR), STEP, DIR)
AccelStepper stepper1(AccelStepper::FULL2WIRE, 5, 6);
AccelStepper stepper2(AccelStepper::FULL2WIRE, 7, 8);

// Creates a group for steppers motors to be added
MultiStepper steppers;

// Forward declare functions
String parseDataStream();

void setup() {
  Serial.begin(9600);

  // Configure each stepper 100 steps per second
  stepper1.setMaxSpeed(MAX_MOTOR_SPEED);
  stepper2.setMaxSpeed(MAX_MOTOR_SPEED);

  // Add them to MultiStepper group to manage
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
}
void loop() {
  
}

String parseDataStream(){
  if (Serial.available()){
    parsedData = Serial.readStringUntil('\n');
    parsedData.trim();
  }
  return parsedData;
}