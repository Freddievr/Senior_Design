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
#define DIRECTION_CCW -1
#define DIRECTION_CW   1

#define MAX_POSITION 150000 // maximum of position we can set (long type)

ezButton lxr(xRS);
ezButton lxl(xLS);
ezButton lzt(zTS);
ezButton lzb(zBS);

AccelStepper sX(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper sZ(AccelStepper::FULL4WIRE, 6, 7, 8, 9);

int directionX  = DIRECTION_CW;
int directionZ  = DIRECTION_CW;
long targetPosX = 0;
long targetPosZ = 0;

void setup() {
  Serial.begin(9600);

  lxr.setDebounceTime(50); // set debounce time to 50 milliseconds
  lxl.setDebounceTime(50); // set debounce time to 50 milliseconds
  lzt.setDebounceTime(50); // set debounce time to 50 milliseconds
  lzb.setDebounceTime(50); // set debounce time to 50 milliseconds

  sX.setMaxSpeed(80000.0);   // set the maximum speed
  sX.setAcceleration(20000.0); // set acceleration
  sX.setSpeed(80000);         // set initial speed
  sX.setCurrentPosition(0); // set position

  sZ.setMaxSpeed(32000.0);   // set the maximum speed
  sZ.setAcceleration(1000.0); // set acceleration
  sZ.setSpeed(16000);         // set initial speed
  sZ.setCurrentPosition(0); // set position

  targetPosX = directionX * MAX_POSITION;
  targetPosZ = directionZ * MAX_POSITION;
  sX.moveTo(targetPosX);
  sZ.moveTo(targetPosZ);
}

void loop() {
  lxr.loop(); // MUST call the loop() function first
  lxl.loop();
  lzt.loop();
  lzb.loop();

  if (lxr.isPressed() || lxl.isPressed()) {
    Serial.println(F("The limit switch: TOUCHED"));
    directionX *= -1; // change direction
    Serial.print(F("The direction -> "));
    if (directionX == DIRECTION_CW)
      Serial.println(F("CLOCKWISE"));
    else
      Serial.println(F("ANTI-CLOCKWISE"));

    targetPosX = directionX * MAX_POSITION;
    sX.setCurrentPosition(0); // set position
    sX.moveTo(targetPosX);
  }

  if (lzt.isPressed() || lzb.isPressed()) {
    Serial.println(F("The limit switch: TOUCHED"));
    directionZ *= -1; // change direction
    Serial.print(F("The direction -> "));
    if (directionZ == DIRECTION_CW)
      Serial.println(F("CLOCKWISE"));
    else
      Serial.println(F("ANTI-CLOCKWISE"));

    targetPosZ = directionZ * MAX_POSITION;
    sZ.setCurrentPosition(0); // set position
    sZ.moveTo(targetPosZ);
  }

  // without this part, the move will stop after reaching maximum position
  if (sX.distanceToGo() == 0) { // if motor moved to the maximum position
    sX.setCurrentPosition(0);   // reset position to 0
    sX.moveTo(targetPosX);       // move the motor to maximum position again
  }

  // without this part, the move will stop after reaching maximum position
  if (sZ.distanceToGo() == 0) { // if motor moved to the maximum position
    sZ.setCurrentPosition(0);   // reset position to 0
    sZ.moveTo(targetPosZ);       // move the motor to maximum position again
  }

  sX.run(); // MUST be called in loop() function
  sZ.run();
}
