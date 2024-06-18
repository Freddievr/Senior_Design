#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <string.h>
#include <Stream.h>

// Defined Variables
#define MAX_MOTOR_SPEED 2000.0
#define HORZ_HOME_SW 11
#define VERT_HOME_SW 10
#define CALIBRATE_MOTOR_SPEED 1000.0

// Global Variables
String streamData;
String parsedData;
bool homeComplete = false;
long initialHoming = 400;

// Initialize Stepper motors
// Function nomenclature motorName(# of Required Input pin(STEP and DIR), STEP, DIR)
AccelStepper stepper1(AccelStepper::FULL2WIRE, 7, 8);
AccelStepper stepper2(AccelStepper::FULL2WIRE, 6, 5);

// Creates a group for steppers motors to be added
// MultiStepper steppers;

// Forward declare functions
String parseDataStream();
void calibrateHome();

void setup() {
  Serial.begin(9600);

  // Configure each stepper # steps per second
  stepper1.setMaxSpeed(MAX_MOTOR_SPEED);
  stepper2.setMaxSpeed(MAX_MOTOR_SPEED);

  // Add them to MultiStepper group to manage
  // steppers.addStepper(stepper1);
  // steppers.addStepper(stepper2);

  // Configure button settings
  pinMode(HORZ_HOME_SW, INPUT_PULLUP);
  pinMode(VERT_HOME_SW, INPUT_PULLUP);
  stepper1.setCurrentPosition(0);
}
void loop() {
  // calibrateHome();
  // stepper1.setCurrentPosition(0);
  stepper1.moveTo(initialHoming);
  // stepper1.setMaxSpeed(CALIBRATE_MOTOR_SPEED);
  stepper1.setSpeed(CALIBRATE_MOTOR_SPEED);
  // stepper1.setAcceleration(CALIBRATE_MOTOR_SPEED);
  // stepper1.move(initialHoming);
  // initialHoming -= 50;
  stepper1.run();
  delay(5);
}

void calibrateHome(){
  while (digitalRead(HORZ_HOME_SW)){
    stepper1.setMaxSpeed(CALIBRATE_MOTOR_SPEED);
    stepper1.setSpeed(CALIBRATE_MOTOR_SPEED);
    stepper1.move(initialHoming);
    initialHoming -= 50;
    stepper1.run();
    delay(5);
  }

  stepper1.setCurrentPosition(0);
  stepper1.setMaxSpeed(CALIBRATE_MOTOR_SPEED);
  // stepper1.setAcceleration(CALIBRATE_MOTOR_SPEED);
  initialHoming = 1;
  
  while (!digitalRead(HORZ_HOME_SW)){
    stepper1.moveTo(initialHoming);
    initialHoming++;
    stepper1.run();
    delay(5);
  }
  stepper1.setCurrentPosition(0);
  
  // initialHoming = -1;
  // while (digitalRead(VERT_HOME_SW)){
  //   stepper2.setSpeed(CALIBRATE_MOTOR_SPEED);
  //   stepper2.moveTo(initialHoming);
  //   initialHoming--;
  //   stepper2.run();
  //   delay(5);
  // }

  // stepper2.setCurrentPosition(0);
  // stepper2.setSpeed(CALIBRATE_MOTOR_SPEED);
  // stepper2.setAcceleration(CALIBRATE_MOTOR_SPEED);
  // initialHoming = 1;
  
  // while (!digitalRead(VERT_HOME_SW)){
  //   stepper2.moveTo(initialHoming);
  //   initialHoming++;
  //   stepper2.run();
  //   delay(5);
  // }
  // stepper2.setCurrentPosition(0);
}

String parseDataStream(){
  if (Serial.available()){
    parsedData = Serial.readStringUntil('\n');
    parsedData.trim();
  }
  return parsedData;
}