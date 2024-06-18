#define pulPinMotor1 8
#define dirPinMotor1 7
#define pulPinMotor2 13
#define dirPinMotor2 4
#define stepsPerRev 800
#define stepperSpeed 500
#define FOR 1
#define BAC 0
float Gap_Width;
float Num_Fingers;

void stepperRun(int steps, int direction, int pulPin, int dirPin);

void setup() {
  pinMode(pulPinMotor1, OUTPUT);    // set pin as PUL
  pinMode(dirPinMotor1, OUTPUT);    // set pin as DIR
  pinMode(pulPinMotor2, OUTPUT);    // set pin as PUL
  pinMode(dirPinMotor2, OUTPUT);    // set pin as DIR
  Serial.begin(9600);

}
void loop() {
  if (!Serial.available()){
  //Do Nothing
  return;
  }
  if (Serial.available()){

  Gap_Width = Serial.parseFloat();
  Num_Fingers = Serial.parseFloat();
  
  Serial.println(Gap_Width);
  Serial.println(Num_Fingers);

  //delay(1000);
    for(float i = 0; i < Num_Fingers; i++){   
      delay(1000);
      stepperRun(stepsPerRev, BAC, pulPinMotor1, dirPinMotor1);
      stepperRun(stepsPerRev, FOR, pulPinMotor1, dirPinMotor1);
      stepperRun(stepsPerRev, BAC, pulPinMotor2, dirPinMotor2);
      stepperRun(stepsPerRev, FOR, pulPinMotor2, dirPinMotor2);
  }

    
  }
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