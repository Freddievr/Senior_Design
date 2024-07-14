#include "AccelStepper.h"

// AccelStepper Setup
AccelStepper stepperX(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper stepperZ(AccelStepper::FULL4WIRE, 6, 7, 8, 9);
// Define the Pins used
#define xRS 10                // Pin number of right limit switch
#define xLS 11                // Pin number of left limit switch
#define zTS 12                // Pin number of top limit switch
#define zBS 13                // Pin number of bottom limit switch
#define maxSpd 2000           // Max spped
#define normSpd 1500          // Normal run speed
#define acclSpd 1000          // Max acceleration speed
#define STEP_PER_MM 320       // Step for 1mm movement
#define MM_PER_STEP 0.003125  // mm movement for 1 step

// Forward declaration
void calibrateHome();


void setup() {
  Serial.begin(9600);
  // Initialized pinmodes for buttons
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

  stepperX.run();
}

void calibrateHome() {

  // Move vertical motor to top limit switch
  // Set zero position on top limit switch
  // Move horizontal motor to right limit switch
  // Set right limit switch as zero position on horizontal switch
  // Move horizontal to left limit switch
  // Save current position of horizontal motor as max range
  // Move to zero position
  // Move vertical down to bottom limit switch
  // Save current position of vertical motor
  // Move to zero position
}

// String parseDataStream(){

// }
