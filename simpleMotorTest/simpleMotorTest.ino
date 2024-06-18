#define pulPinMotor1 8
#define dirPinMotor1 7
#define pulPinMotor2 13
#define dirPinMotor2 4
#define stepsPerRev 800
#define stepperSpeed 500
#define FOR 1
#define BAC 0

void stepperRun(int steps, int direction, int pulPin, int dirPin);

void setup() {
  pinMode(pulPinMotor1, OUTPUT);    // set pin as PUL
  pinMode(dirPinMotor1, OUTPUT);    // set pin as DIR
  pinMode(pulPinMotor2, OUTPUT);    // set pin as PUL
  pinMode(dirPinMotor2, OUTPUT);    // set pin as DIR
}
void loop() {
  while (1){

  }
  stepperRun(stepsPerRev, BAC, pulPinMotor1, dirPinMotor1);
  stepperRun(stepsPerRev, FOR, pulPinMotor1, dirPinMotor1);
  stepperRun((stepsPerRev), BAC, pulPinMotor2, dirPinMotor2);
  stepperRun((stepsPerRev), FOR, pulPinMotor2, dirPinMotor2);
 
}

void stepperRun(int steps, int direction, int pulPin, int dirPin){  
  digitalWrite(dirPin, direction);  // set direction level
  for (int x = 0; x < steps; x++)   // repeat "steps" times a revolution
  {
    digitalWrite(pulPin, HIGH);     // Output high
    delayMicroseconds(stepperSpeed);// set rotate speed
    digitalWrite(pulPin, LOW);      // Output low
    delayMicroseconds(stepperSpeed);// set rotate speed
  }
  delay(1000);                      // May be unnecessary
}