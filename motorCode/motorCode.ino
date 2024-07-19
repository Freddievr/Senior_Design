/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-stepper-motor-and-limit-switch
 */

#include <ezButton.h>
#include <AccelStepper.h>

#define xRS 13  // Pin 12 connected to Home Switch (MicroSwitch)
#define xLS 12
#define zTS 11  // Pin 12 connected to Home Switch (MicroSwitch)
#define zBS 10
#define AWAY_MOTOR -1
#define TOWARD_MOTOR 1
#define MAX_POSITION 150000  // maximum of position we can set (long type)
#define MAX_SPD 3000
#define NORM_SPD 2000
#define ACCL_SPD 3000
#define STEP_PER_MM 320
#define MM_PER_STEP 0.003125
#define Z_MEASURE_STEP 4000

ezButton lxr(xRS);
ezButton lxl(xLS);
ezButton lzt(zTS);
ezButton lzb(zBS);

AccelStepper sX(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper sZ(AccelStepper::FULL4WIRE, 6, 7, 8, 9);

void setMotorSpd(int selMotor, int maxSpd, int normSpd, int acclSpd);
void moveXHome();
void moveZTopHome();
void moveStpr(int selStpr, int steps, int direction);

long targetPosX = 0;
long targetPosZ = 0;
bool homeXPos = false;
bool homeZTopPos = false;
bool homeZBotPos = false;
bool probeContact = false;
bool measuringZInProg = false;
unsigned long previousMillis = 0;
const long interval = 2000;

void setup() {
  Serial.begin(9600);

  lxr.setDebounceTime(50);  // set debounce time to 50 milliseconds
  lxl.setDebounceTime(50);  // set debounce time to 50 milliseconds
  lzt.setDebounceTime(50);  // set debounce time to 50 milliseconds
  lzb.setDebounceTime(50);  // set debounce time to 50 milliseconds

  setMotorSpd(3, MAX_SPD, NORM_SPD, ACCL_SPD);
  sX.setCurrentPosition(0);  // set position
  sZ.setCurrentPosition(0);  // set position

  // targetPosX = TOWARD_MOTOR * MAX_POSITION;
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
  if (homeXPos == false) {
    moveXHome();
  }
  if (homeZTopPos == false) {
    moveZTopHome();
  }
  if (homeZTopPos == true && measuringZInProg == false) {
    sZ.move(1600);
  }
  if (measuringZInProg == false) {
    moveZBotHome();
  }
  if (measuringZInProg == true && sZ.distanceToGo() == 0) {
    sZ.moveTo(Z_MEASURE_STEP);
  }
  if (measuringZInProg == true) {
    sX.run();
    sZ.run();
  }
  if (measuringZInProg == true && sZ.distanceToGo() == 0) {
    measureZ();
  }

}

void setMotorSpd(int selMotor, int maxSpd, int normSpd, int acclSpd) {
  if (selMotor == 1) {
    sX.setMaxSpeed(maxSpd);       // set the maximum speed
    sX.setAcceleration(acclSpd);  // set acceleration
    sX.setSpeed(normSpd);         // set initial speed
  }
  if (selMotor == 1) {
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
  }
  if (homeZBotPos == false && measuringZInProg == false) {
    sZ.run();
  }
}

void measureZ() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sZ.moveTo(0);
  }
}