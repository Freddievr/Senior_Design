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
#define stepsHoriz 800
#define stepsVert 400
#define stepperCalibrateSpeed 500
#define stepperRunSpeed 600
#define FOR 1
#define BAC 0

// Global Variables
bool homeComplete = false;
int initialHoming[2];
char myData[30] = {0};
int currFinger;
int numFingers;

// Forward declare functions
void parseDataStream();
void calibrateHome();
void stepperRun(int steps, int direction, int stepperSpeed, int pulPin, int dirPin);


void setup() {
 // Configure button settings
  pinMode(HORZ_HOME_SW, INPUT_PULLUP);
  pinMode(VERT_HOME_SW, INPUT_PULLUP);
  pinMode(PUL_PIN_MOTOR_1, OUTPUT);  // set pin as PUL
  pinMode(DIR_PIN_MOTOR_1, OUTPUT);  // set pin as DIR
  pinMode(PUL_PIN_MOTOR_2, OUTPUT);  // set pin as PUL
  pinMode(DIR_PIN_MOTOR_2, OUTPUT);  // set pin as DIR

  Serial.begin(9600);
  Serial.flush();
}

void loop() {
  parseDataStream();
  
  //resetHoriz(2000);
  if (!homeComplete){
  calibrateHome();
  }
  for(currFinger = 0; currFinger < numFingers; currFinger++){     
      delay(500);

      stepperRun(stepsHoriz, BAC, stepperRunSpeed, PUL_PIN_MOTOR_1, DIR_PIN_MOTOR_1);
      stepperRun(stepsVert, FOR, stepperRunSpeed, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
      stepperRun(stepsVert, BAC, stepperRunSpeed, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
      //Serial.print("Finger: #"); Serial.println(Curr_Finger+1);
      //Serial.print("Steps"); Serial.println(stepsPerRev);
      delay(1000);
      if ((currFinger+1) == numFingers){
        delay(1000);        //Serial.print("Measurements Done");
        exit(0);


        
      }
  }
}

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

void resetHoriz(int steps){  
  digitalWrite(DIR_PIN_MOTOR_1, BAC);      // set direction level
  for (int x = 0; x < steps; x++){      // repeat "steps" times a revolution
    digitalWrite(PUL_PIN_MOTOR_1, HIGH);   // Output high
    delayMicroseconds(stepperRunSpeed);    // set rotate speed
    digitalWrite(PUL_PIN_MOTOR_1, LOW);    // Output low
    delayMicroseconds(stepperRunSpeed);    // set rotate speed
  }
  delay(1000);                      // May be unnecessary
}

void resetVert(int steps){  
  digitalWrite(DIR_PIN_MOTOR_2, FOR);      // set direction level
  for (int x = 0; x < steps; x++){      // repeat "steps" times a revolution
    digitalWrite(PUL_PIN_MOTOR_1, HIGH);   // Output high
    delayMicroseconds(stepperRunSpeed);    // set rotate speed
    digitalWrite(PUL_PIN_MOTOR_1, LOW);    // Output low
    delayMicroseconds(stepperRunSpeed);    // set rotate speed
  }
  delay(1000);                      // May be unnecessary
}

void parseDataStream() {
  byte n = Serial.available();
  if (n != 0) {
    byte m = Serial.readBytesUntil('\n', myData, 30);
    myData[m] = '\0';  //null-byte

    float gapWidth = atof(strtok(myData, ","));             // Separates string using "," as delimiter
    numFingers = atoi(strtok(NULL, ","));             // Converts ASCII number to integer
    float calculatedSteps = (gapWidth / 5.0) * 1600;  // Calculates steps to move a certain distance
    Serial.print(gapWidth);
    Serial.print(",");
    Serial.println(numFingers);
    delay(1000);
    Serial.end();
  }
}

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
  delay(500);
  initialHoming[0] = 0;  // Sets initial position for horizontal
  
  //Begins calibration for vertical
  //while (digitalRead(VERT_HOME_SW)) { // Waiting for button press
  //   stepperRun(stepsVert, BAC, stepperCalibrateSpeed, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
  //  delay(5);
  //}
  //while (!digitalRead(VERT_HOME_SW)) { // Coming off button press
  //  stepperRun(stepsVert, FOR, stepperCalibrateSpeed, PUL_PIN_MOTOR_2, DIR_PIN_MOTOR_2);
  //  delay(5);
  // }

  //initialHoming[1] = 0;  // Sets initial position for vertical
  homeComplete = true;   // Completes calibration
}

  
