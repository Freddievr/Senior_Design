#include <Arduino.h>
#include <string.h>
#include <Stream.h>

// Defined Variables
#define MAX_MOTOR_SPEED 2000
#define HORZ_HOME_SW 11
#define VERT_HOME_SW 10
#define CALIBRATE_MOTOR_SPEED 1000
#define PUL_PIN_MOTOR_1 8
#define DIR_PIN_MOTOR_1 7
#define PUL_PIN_MOTOR_2 13
#define DIR_PIN_MOTOR_2 4
#define stepsPerRev 800
#define stepperCalibrateSpeed 500
#define FOR 1
#define BAC 0

// Global Variables
bool homeComplete = false;
int initialHoming[2];
char myData[30] = {0};

// Forward declare functions
void parseDataStream();
void calibrateHome();
void stepperRun(int steps, int direction, int stepperSpeed, int pulPin, int dirPin);

void setup() {
  Serial.begin(9600);

  // Configure button settings
  pinMode(HORZ_HOME_SW, INPUT_PULLUP);
  pinMode(VERT_HOME_SW, INPUT_PULLUP);
  pinMode(PUL_PIN_MOTOR_1, OUTPUT);  // set pin as PUL
  pinMode(DIR_PIN_MOTOR_1, OUTPUT);  // set pin as DIR
  pinMode(PUL_PIN_MOTOR_2, OUTPUT);  // set pin as PUL
  pinMode(DIR_PIN_MOTOR_2, OUTPUT);  // set pin as DIR
}
void loop() {
  calibrateHome();
}

/*=============================================================== 
Finds the home position by backing slowly into the limit switch.
Once it detects a button press it should stop and slowly back off
until switch is open again. This will be the "initialHoming"
position
================================================================*/
void calibrateHome() {
  if (homeComplete){
    
  }
  // Moves slowly into switch is not active
  while (digitalRead(HORZ_HOME_SW)) { // Waiting for button press
    stepperRun(stepsPerRev, BAC, stepperCalibrateSpeed, PUL_PIN_MOTOR_1, DIR_PIN_MOTOR_1);
    delay(5);
  }
  // Slowly backs off switch until its open
  while (!digitalRead(HORZ_HOME_SW)) { // Coming off button press
    stepperRun(stepsPerRev, FOR, stepperCalibrateSpeed, PUL_PIN_MOTOR_1, DIR_PIN_MOTOR_1);
    delay(5);
  }
  delay(500);
  initialHoming[0] = 0;  // Sets initial position for horizontal
  
  // Begins calibration for vertical
  while (digitalRead(VERT_HOME_SW)) { // Waiting for button press
    stepperRun(stepsPerRev, BAC, stepperCalibrateSpeed, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
    delay(5);
  }
  while (!digitalRead(VERT_HOME_SW)) { // Coming off button press
    stepperRun(stepsPerRev, FOR, stepperCalibrateSpeed, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
    delay(5);
  }
  initialHoming[1] = 0;  // Sets initial position for vertical
  homeComplete = true;   // Completes calibration
}

/*=============================================================== 
Sending a pulse to the stepper motor by hand is done my making a
PWM signal. The number of signals for a single rotation of the
stepper motor is dependant on the motor driver settings
steps: how many times to step the motor
direction: direction motor spins typically CW or CCW
pulPin: pin for pulsing the motor sent to the motor driver
dirPin: pin for setting motor direction on motor driver
================================================================*/
void stepperRun(int steps, int direction, int stepperSpeed, int pulPin, int dirPin) {
  digitalWrite(dirPin, direction);    // set direction level
  for (int x = 0; x < steps; x++) {   // repeat "steps" times a revolution
    digitalWrite(pulPin, HIGH);       // Output high
    delayMicroseconds(stepperSpeed);  // set rotate speed
    digitalWrite(pulPin, LOW);        // Output low
    delayMicroseconds(stepperSpeed);  // set rotate speed
  }
  delay(1000);  // Maybe unnecessary
}

void parseDataStream() {
  byte n = Serial.available();
  if (n != 0) {
    byte m = Serial.readBytesUntil('\n', myData, 30);
    myData[m] = '\0';  //null-byte

    float gapWidth = atof(strtok(myData, ","));             // Separates string using "," as delimiter
    float calculatedSteps = (gapWidth / 5.0) * 1600;  // Calculates steps to move a certain distance
    int numFingers = atoi(strtok(NULL, ","));             // Converts ASCII number to integer
    Serial.print(gapWidth);
    Serial.print(",");
    Serial.println(calculatedSteps);
    delay(1000);
    Serial.end();
  }
}