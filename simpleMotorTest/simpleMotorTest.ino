#define pulPinMotor 8
#define dirPinMotor 7
// #define pulPinMotor2 13
// #define dirPinMotor2 4
#define stepsPerRev 800
#define stepperSpeed 700
#define FOR 1
#define BAC 0

void stepperRun(int steps, int direction);

void setup() {
  pinMode(pulPinMotor, OUTPUT);  // set Pin9 as PUL
  pinMode(dirPinMotor, OUTPUT);  // set Pin8 as DIR
}
void loop() {
  // stepperRun(stepsPerRev, FOR);
  // stepperRun(stepsPerRev, BAC, pulPinMotor1, dirPinMotor1);
  stepperRun(stepsPerRev, FOR);
  // stepperRun(stepsPerRev, BAC, pulPinMotor2, dirPinMotor2);
  // stepperRun(stepsPerRev, FOR, pulPinMotor2, dirPinMotor2);
  // digitalWrite(dirPin, test);     // set high level direction
  // for (x = 0; x < stepsPerRev; x++)  // repeat 400 times a revolution when setting 400 on driver
  // {
  //   digitalWrite(pulPin, HIGH);   // Output high
  //   delayMicroseconds(stepperSpeed);  // set rotate speed
  //   digitalWrite(pulPin, LOW);    // Output low
  //   delayMicroseconds(stepperSpeed);  // set rotate speed
  // }
  // delay(1000);           //pause 1 second
  // digitalWrite(dirPin, BAC);  // set high level direction
  // for (x = 0; x < stepsPerRev; x++) {
  //   digitalWrite(pulPin, HIGH);
  //   delayMicroseconds(stepperSpeed);
  //   digitalWrite(pulPin, LOW);
  //   delayMicroseconds(stepperSpeed);
  // }
  // delay(1000);
}

void stepperRun(int steps, int direction){  
  digitalWrite(dirPinMotor, direction);     // set high level direction
  for (int x = 0; x < steps; x++)  // repeat 400 times a revolution when setting 400 on driver
  {
    digitalWrite(pulPinMotor, HIGH);   // Output high
    delayMicroseconds(stepperSpeed);  // set rotate speed
    digitalWrite(pulPinMotor, LOW);    // Output low
    delayMicroseconds(stepperSpeed);  // set rotate speed
  }
  delay(1000); // May be unnecessary
}