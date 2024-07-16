#include "AccelStepper.h"

// AccelStepper Setup
AccelStepper stepperX(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper stepperZ(AccelStepper::FULL4WIRE, 6, 7, 8, 9);
// Define the Pins used
#define xRS 10                // Pin number of right limit switch
#define xLS 11                // Pin number of left limit switch
#define zTS 12                // Pin number of top limit switch
#define zBS 13                // Pin number of bottom limit switch
#define maxSpd 5000           // Max spped
#define normSpd 3000          // Normal run speed
#define acclSpd 3000          // Max acceleration speed
#define STEP_PER_MM 320       // Step for 1mm movement
#define MM_PER_STEP 0.003125  // mm movement for 1 step
// Global Variables
int negAxis = 0;
const byte numChars = 32;
char receivedChars[numChars];  // an array to store the received data
boolean newData = false;
int dataNumber = 0;  // new for this version

// Forward declaration
void calibrateHome();
void measureSample();


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

  calibrateHome();  // Home localization
}

void loop() {
  // recvWithEndMarker();
  // showNewNumber();
  // measureSample();

  // while (digitalRead(zTS) == 1) {  // Make the Stepper move CW until the switch is activated
  //   stepperZ.move(-1600);          // Set the position to move to
  //   stepperZ.runSpeed();           // Start moving the stepper
  // }
}

void calibrateHome() {
  // Move vertical motor to top limit switch
  while (digitalRead(zTS) == 0) {  // Make the Stepper move CCW until the switch is activated
    stepperZ.move(-1600);          // Set the position to move to
    stepperZ.runSpeed();           // Start moving the stepper
  }
  stepperZ.setCurrentPosition(0);
  stepperZ.runToNewPosition(800);
  stepperZ.setCurrentPosition(0);  // Set zero position on top limit switch
  // Move horizontal motor to right limit switch
  while (digitalRead(xRS) == 0) {  // Make the Stepper move CW until the switch is activated
    stepperX.move(1600);           // Set the position to move to
    stepperX.runSpeed();           // Start moving the stepper
  }
  stepperX.setCurrentPosition(0);
  stepperX.runToNewPosition(-800);
  stepperX.setCurrentPosition(0);  // Set right limit switch as zero position on horizontal switch
  // Move horizontal to left limit switch
  while (digitalRead(xLS) == 0) {  // Make the Stepper move CCW until the switch is activated
    stepperX.move(-1600);          // Set the position to move to
    stepperX.runSpeed();           // Start moving the stepper
  }
  // Move vertical to calibrate contact
  while (digitalRead(zBS) == 0) {  // Make the Stepper move CW until the switch is activated
    stepperZ.move(1600);           // Set the position to move to
    stepperZ.runSpeed();           // Start moving the stepper
  }
  stepperZ.setCurrentPosition(0);    // Set vertical contact position
  stepperZ.runToNewPosition(-4800);  // Move vertical off contact position
  stepperX.runToNewPosition(0);      // Move to horizontal to zero position
}
void measureSample() {
  if (stepperX.distanceToGo() == 0 && negAxis < dataNumber) {
    stepperX.move(-STEP_PER_MM);
    negAxis++;
    stepperZ.runToNewPosition(-4800);
    stepperZ.runToNewPosition(0);
    delay(500);
    stepperZ.runToNewPosition(-4800);
    delay(1000);
  }
  stepperX.run();
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  if (Serial.available() > 0) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    } else {
      receivedChars[ndx] = '\0';  // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewNumber() {
  if (newData == true) {
    dataNumber = 0;
    dataNumber = atoi(receivedChars);
    Serial.println(dataNumber);
    newData = false;
  }
}