#include <ezButton.h>
#include <AccelStepper.h>

#define xRS 13  // Pin 12 connected to Home Switch (MicroSwitch)
#define xLS 12
#define zTS 11  // Pin 12 connected to Home Switch (MicroSwitch)
#define zBS 10
#define AWAY_MOTOR -1
#define TOWARD_MOTOR 1
#define MAX_POSITION 150000  // maximum of position we can set (long type)
#define MAX_SPD 50000
#define NORM_SPD 35000
#define ACCL_SPD 35000
#define STEP_PER_MM 1284
#define MM_PER_STEP 0.003125
#define Z_MEASURE_STEP 3500 // 4000 Original

ezButton lxr(xRS);
ezButton lxl(xLS);
ezButton lzt(zTS);
ezButton lzb(zBS);

AccelStepper sX(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper sZ(AccelStepper::FULL4WIRE, 6, 7, 8, 9);

void setMotorSpd(int selMotor, int maxSpd, int normSpd, int acclSpd);
void moveXHome();
void moveZTopHome();
void moveZBotHome();
void measureZ(float gapWidth);
void homeCalibration();
void checkDataStream();
void recvWithStartEndMarkers();
void parseData();
void showParseData();
void resetUno();
void checkFlags();

long targetPosX = 0;
long targetPosZ = 0;
bool homeXPos = false;
bool homeZTopPos = false;
bool homeZBotPos = false;
bool measuringZInProg = false;
bool calComplete = false;
int time = 7600;
int stepSize;
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];  // temporary array for use when parsing
int numFingers;
int currFinger;
int calculatedStepSize;

// variables to hold the parsed data
char messageFromPC[numChars] = { 0 };
int numFingersFromPC = 0;
int gapStepsFromPC = 0;
boolean newData = false;

void setup() {
  Serial.begin(9600);

  lxr.setDebounceTime(50);  // set debounce time to 50 milliseconds
  lxl.setDebounceTime(50);  // set debounce time to 50 milliseconds
  lzt.setDebounceTime(50);  // set debounce time to 50 milliseconds
  lzb.setDebounceTime(50);  // set debounce time to 50 milliseconds

  setMotorSpd(3, MAX_SPD, NORM_SPD, ACCL_SPD);
  sX.setCurrentPosition(0);  // set position
  sZ.setCurrentPosition(0);  // set position

  targetPosX = TOWARD_MOTOR * MAX_POSITION;
  targetPosZ = AWAY_MOTOR * MAX_POSITION;
  sX.move(targetPosX);
  sZ.move(targetPosZ);
}

void loop() {
  lxr.loop();  // MUST call the loop() function first
  lxl.loop();
  lzt.loop();
  lzb.loop();
  checkDataStream();
  checkFlags();
  if (calComplete == false && messageFromPC[0] == 'V') {
    homeCalibration();
  }
  if (calComplete == true && homeZBotPos == true) {
    Serial.print('K');
    delay(500);
}
  if (calComplete == true) {
    delay(6000);
    for(currFinger; currFinger < numFingers; currFinger++){
      measureZ(calculatedStepSize);  //Desired Travel Length/5mm * step resolution (1600)
    } 
  }
  // resetUno();
}

void setMotorSpd(int selMotor, int maxSpd, int normSpd, int acclSpd) {
  if (selMotor == 1) {
    sX.setMaxSpeed(maxSpd);       // set the maximum speed
    sX.setAcceleration(acclSpd);  // set acceleration
    sX.setSpeed(normSpd);         // set initial speed
  }
  if (selMotor == 2) {
    sZ.setMaxSpeed(maxSpd);       // set the maximum speed
    sZ.setAcceleration(acclSpd);  // set acceleration
    sZ.setSpeed(normSpd);         // set initial speed
  }
  if (selMotor == 3) {
    sX.setMaxSpeed(maxSpd);       // set the maximum speed
    sX.setAcceleration(acclSpd);  // set acceleration
    sX.setSpeed(normSpd);         // set initial speed
    sZ.setMaxSpeed(maxSpd);       // set the maximum speed
    sZ.setAcceleration(acclSpd);  // set acceleration
    sZ.setSpeed(normSpd);
  }
}

void homeCalibration() {
  if (homeXPos == false) {
    moveXHome();
  }
  if (homeZTopPos == false) {
    moveZTopHome();
  }
  if (homeZTopPos == true && measuringZInProg == false) {
    sZ.moveTo(MAX_POSITION);
  }
  if (measuringZInProg == false && homeXPos == true) {
    moveZBotHome();
  }
}

void moveXHome() {
  if (lxr.isPressed()) {
    sX.setCurrentPosition(0);  // set position
    sX.move(0);
    homeXPos = true;
  }
  if (homeXPos == false) {
    sX.run();  // MUST be called in loop() function
  }
}

void moveZTopHome() {
  if (lzt.isPressed()) {
    sZ.setCurrentPosition(0);  // set position
    sZ.move(0);
    homeZTopPos = true;
  }
  if (homeZTopPos == false) {
    sZ.run();
  }
}

void moveZBotHome() {
  if (lzb.isPressed()) {
    sZ.setCurrentPosition(0);  // set position
    sZ.move(0);
    homeZBotPos = true;
    measuringZInProg = true;
    calComplete = true;
  }
  if (homeZBotPos == false && measuringZInProg == false) {
    sZ.run();
  }
}

void measureZ(float gapWidth) {
  sZ.moveTo(3000);
  sZ.runToPosition();
  delay(time);
  stepSize += gapWidth;
  sZ.moveTo(0);
  sZ.runToPosition();
  sX.moveTo(-stepSize);
  sX.runToPosition();
  // currFinger++;
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedChars[ndx] = '\0';  // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void parseData() {   // split the data into its parts
  char* strtokIndx;  // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, ",");  // get the first part - the string
  strcpy(messageFromPC, strtokIndx);    // copy it to messageFromPC

  strtokIndx = strtok(NULL, ",");
  gapStepsFromPC = atoi(strtokIndx);  // convert this part to a float

  strtokIndx = strtok(NULL, ",");    // this continues where the previous call left off
  numFingersFromPC = atoi(strtokIndx);  // convert this part to an integer
}
void checkDataStream() {
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    newData = false;
  }
}

void resetUno() {  // Resets Parameters for Arduino Uno
  numFingers = 0;
  currFinger = 0;
  targetPosX = 0;
  targetPosZ = 0;
  homeXPos = false;
  homeZTopPos = false;
  homeZBotPos = false;
  measuringZInProg = false;
  calComplete = false;
}

void checkFlags() {
  switch (messageFromPC[0]) {
    case 'C':
      calComplete = false;
      break;
    case 'V':
      numFingers = numFingersFromPC;
      calculatedStepSize = gapStepsFromPC;
      break;
  }
}