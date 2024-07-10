#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <string.h>
#include <Stream.h>

// Defined Variables
#define MAX_MOTOR_SPEED 100
#define HORZ_HOME_SW 11
#define VERT_HOME_SW 10
#define CALIBRATE_MOTOR_SPEED 1000.0

// Global Variables
String streamData;
String parsedData;
bool homeComplete = false;
long initialHoming = 400;

// Initialize Stepper motors
// Function nomenclature motorName
AccelStepper stepperH(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper stepperV(AccelStepper::FULL4WIRE, 6, 7, 8, 9);

// Creates a group for steppers motors to be added
MultiStepper steppers;
// MultiStepper steppers;

// Forward declare functions
// String parseDataStream();
void calibrateHome();

void setup() {
  Serial.begin(9600);

  // Configure each stepper # steps per second
  stepperH.setMaxSpeed(MAX_MOTOR_SPEED);
  stepperV.setMaxSpeed(MAX_MOTOR_SPEED);

  // Add them to MultiStepper group to manage
  // steppers.addStepper(stepperH);
  // steppers.addStepper(stepperV);

  // Configure button settings
  pinMode(HORZ_HOME_SW, INPUT_PULLUP);
  pinMode(VERT_HOME_SW, INPUT_PULLUP);
  stepperH.setCurrentPosition(0);
  stepperV.setCurrentPosition(0);
}

void loop() {
  // calibrateHome();
  // stepperH.setCurrentPosition(0);
  stepperH.moveTo(initialHoming);
  // stepperH.setMaxSpeed(CALIBRATE_MOTOR_SPEED);
  stepperH.setSpeed(CALIBRATE_MOTOR_SPEED);
  // stepperH.setAcceleration(CALIBRATE_MOTOR_SPEED);
  // stepperH.move(initialHoming);
  // initialHoming -= 50;
  stepperH.run();
  delay(5);

  /*=======================*/
  // Vert moves down
  // Vert moves up
  // Horiz moves forward
}

void calibrateHome(){

  // While switch is open move horiz towards switch
  // Slowly move off of switch until open after switch is closed
  // Set initial position
  // REPEAT FOR VERT

  while (digitalRead(HORZ_HOME_SW)){
    stepperH.setMaxSpeed(CALIBRATE_MOTOR_SPEED);
    stepperH.setSpeed(CALIBRATE_MOTOR_SPEED);
    stepperH.move(initialHoming);
    initialHoming -= 50;
    stepperH.run();
    delay(5);
  }

  stepperH.setCurrentPosition(0);
  stepperH.setMaxSpeed(CALIBRATE_MOTOR_SPEED);
  // stepperH.setAcceleration(CALIBRATE_MOTOR_SPEED);
  initialHoming = 1;
  
  while (!digitalRead(HORZ_HOME_SW)){
    stepperH.moveTo(initialHoming);
    initialHoming++;
    stepperH.run();
    delay(5);
  }
  stepperH.setCurrentPosition(0);
  
  // initialHoming = -1;
  // while (digitalRead(VERT_HOME_SW)){
  //   stepperV.setSpeed(CALIBRATE_MOTOR_SPEED);
  //   stepperV.moveTo(initialHoming);
  //   initialHoming--;
  //   stepperV.run();
  //   delay(5);
  // }

  // stepperV.setCurrentPosition(0);
  // stepperV.setSpeed(CALIBRATE_MOTOR_SPEED);
  // stepperV.setAcceleration(CALIBRATE_MOTOR_SPEED);
  // initialHoming = 1;
  
  // while (!digitalRead(VERT_HOME_SW)){
  //   stepperV.moveTo(initialHoming);
  //   initialHoming++;
  //   stepperV.run();
  //   delay(5);
  // }
  // stepperV.setCurrentPosition(0);
}

// String parseDataStream(){

// }
