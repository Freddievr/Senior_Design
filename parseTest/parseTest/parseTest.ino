#include <Arduino.h>
#include <string.h>
#include <Stream.h>
#include <TimerOne.h>
// Defined Variables
#define FOR HIGH
#define BAC LOW
#define UP HIGH
#define DOWN LOW
#define PUL_PIN_MOTOR_1 8
#define DIR_PIN_MOTOR_1 7
#define PUL_PIN_MOTOR_2 13
#define DIR_PIN_MOTOR_2 4
#define HORZ_HOME_SW 11
#define VERT_HOME_SW 10
#define MAX_MOTOR_SPEED 2000
#define CALIBRATE_MOTOR_SPEED 1200
#define stepsHoriz 100      // Horizontal Calibration Distance 
#define stepsVert 100        // Vertical Set Calibration & Measurement Movement Distance
#define calcStepsHoriz 300  // Vertical Calculated Distance
#define calcStepsVert 700   // Horizontal Calculated Distance (Moving for Testing )
#define stepperCalibrateSpeed 400
#define stepperRunSpeed 400
#define stepperRunSpeedHoriz 700
// Global Variables

// Global Variables
bool homeComplete = false;
bool parsed = false;
int initialHoming[2];
char receivedChars[30] = {0};
const byte numChars = 30;
int currFinger;
int numFingers;
float calculatedSteps;

static boolean recvInProgress = false;
static byte ndx = 0;
char startMarker = '<';
char endMarker = '>';
char recieved;

boolean newData = false;

// Forward declare functions
void parseDataStream();
void calibrateHome();
void stepperRun(int steps, int direction, int stepperSpeed, int pulPin, int dirPin);

void setup() {
  //Timer1.initialize(100000);
  //Timer1.attachInterrupt(parseDataStream);
  // Setups parsing interrupt
  Serial.begin(9600);
  Serial.flush();
  // Configure button settings
  pinMode(HORZ_HOME_SW, INPUT_PULLUP);
  pinMode(VERT_HOME_SW, INPUT_PULLUP);
  pinMode(PUL_PIN_MOTOR_1, OUTPUT);  // set pin as PUL
  pinMode(DIR_PIN_MOTOR_1, OUTPUT);  // set pin as DIR
  pinMode(PUL_PIN_MOTOR_2, OUTPUT);  // set pin as PUL
  pinMode(DIR_PIN_MOTOR_2, OUTPUT);  // set pin as DIR
}
void loop() {
  //resetHoriz(2000);
  //resetUno();
  parseDataStream();

  if (!homeComplete){
  Serial.println("starting... ");
  calibrateHome();
  }

  do{    
    Serial.println(currFinger+1);
      if (currFinger == 0){
        stepperRun(calcStepsVert, DOWN, stepperRunSpeedHoriz, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
        delay(100);// Measure Finger 1
        stepperRun(calcStepsVert, UP, stepperRunSpeed, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
        currFinger += 1;
      }
      else if (currFinger > 0){
        stepperRun(calculatedSteps, BAC, stepperRunSpeed, PUL_PIN_MOTOR_1, DIR_PIN_MOTOR_1);
        stepperRun(calcStepsVert, DOWN, stepperRunSpeedHoriz, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
        delay(100);//Measure Finger n
        stepperRun(calcStepsVert, UP, stepperRunSpeed, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
        currFinger += 1;
      }
} while(currFinger < numFingers);
Serial.println("DONE!");
resetUno();         // Resets Arduino Parameters 
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
}

void resetHoriz(int steps){  
  digitalWrite(DIR_PIN_MOTOR_1, BAC);      // set direction level
  for (int x = 0; x < steps; x++){      // repeat "steps" times a revolution
    digitalWrite(PUL_PIN_MOTOR_1, HIGH);   // Output high
    delayMicroseconds(stepperRunSpeed);    // set rotate speed
    digitalWrite(PUL_PIN_MOTOR_1, LOW);    // Output low
    delayMicroseconds(stepperRunSpeed);    // set rotate speed
  }
}

void resetVert(int steps){  
  digitalWrite(DIR_PIN_MOTOR_2, FOR);      // set direction level
  for (int x = 0; x < steps; x++){      // repeat "steps" times a revolution
    digitalWrite(PUL_PIN_MOTOR_1, HIGH);   // Output high
    delayMicroseconds(stepperRunSpeed);    // set rotate speed
    digitalWrite(PUL_PIN_MOTOR_1, LOW);    // Output low
    delayMicroseconds(stepperRunSpeed);    // set rotate speed
  }
}

void resetUno(){           // Resets Parameters for Arduino Uno
  homeComplete = false;
  numFingers = 0;
  currFinger = 0;
//  calcStepsHoriz = 0;
//  calcStepsVert = 0;
  parsed = false;         
}
/*=============================================================== 
Finds the home position by backing slowly into the limit switch.
Once it detects a button press it should stop and slowly back off
until switch is open again. This will be the "initialHoming"
position
================================================================*/
void calibrateHome() {
  if (homeComplete == true){
    
  }
  // Moves slowly into switch is not active
  while (digitalRead(HORZ_HOME_SW)) { // Waiting for button press
    stepperRun(stepsHoriz, FOR, stepperCalibrateSpeed, PUL_PIN_MOTOR_1, DIR_PIN_MOTOR_1);
    delay(5);
  }
  // Slowly backs off switch until its open
  while (!digitalRead(HORZ_HOME_SW)) { // Coming off button press
    stepperRun(stepsHoriz, BAC, stepperCalibrateSpeed, PUL_PIN_MOTOR_1, DIR_PIN_MOTOR_1);
    delay(5);
  }
  initialHoming[0] = 0;  // Sets initial position for horizontal
  
  //Begins calibration for vertical
  while (digitalRead(VERT_HOME_SW)) { // Waiting for button press
     stepperRun(stepsVert, UP, stepperCalibrateSpeed, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
    delay(5);
  }
  while (!digitalRead(VERT_HOME_SW)) { // Coming off button press
    stepperRun(stepsVert, DOWN, stepperCalibrateSpeed, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
    delay(5);
  }

  initialHoming[1] = 0;  // Sets initial position for vertical
  Serial.println("Home");
  homeComplete = true;   // Completes calibration
}

void parseDataStream() {
 do{
  recieved = Serial.read();
  if (recvInProgress == true) {
    if (recieved != endMarker) {
      receivedChars[ndx] = recieved;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    } 
    else {
      receivedChars[ndx] = '\0';  // terminate the string
      recvInProgress = false;
      ndx = 0;
      float gapWidth = atof(strtok(receivedChars, ","));             // Separates string using "," as delimiter
      numFingers = atoi(strtok(NULL, ","));             // Converts ASCII number to integer
      calculatedSteps = (gapWidth / 5.0) * 1600;  // Calculates steps to move a certain distance
      Serial.println(gapWidth);
      Serial.print(",");
      Serial.println(numFingers);
      parsed = true;
    }
  }
  else if (recieved == startMarker) {
    recvInProgress = true;
  }

Serial.print("parsing... ");
}while(!parsed);
}  

