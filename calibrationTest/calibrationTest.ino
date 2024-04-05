// Calibration button pins
#define buttonPin1 6
#define buttonPin2 5
// Stepper pins
#define dirPinM1 8
#define stepPinM1 9
// Motor positions [initial final]
int steps[2] = {0, 0};
// Motor movement variables
int x;
int motorDistance = 3000;
int motorSpeed = 100; // Lower is faster

void setup() {
  Serial.begin(9600);
  pinMode(stepPinM1, OUTPUT);  // set Pin9 as PUL
  pinMode(dirPinM1, OUTPUT);   // set Pin8 as DIR
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
}
void loop() {
  
  while (digitalRead(buttonPin1)) {
    digitalWrite(dirPinM1, HIGH);
    digitalWrite(stepPinM1, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(stepPinM1, LOW);
    delayMicroseconds(motorSpeed);
  }
  delay(1000);
// Slowly move away from switch until switch is open
  while (!digitalRead(buttonPin1)) {
    digitalWrite(dirPinM1, LOW);
    digitalWrite(stepPinM1, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(stepPinM1, LOW);
    delayMicroseconds(motorSpeed);
  }
  steps[0] = 0;
  Serial.println(steps[0]);
  delay(1000);
// Moves in opposite direction to find max travel distance
  while (digitalRead(buttonPin2)) {
    digitalWrite(dirPinM1, LOW);
    digitalWrite(stepPinM1, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(stepPinM1, LOW);
    delayMicroseconds(motorSpeed);
    steps[1]++;
  }
// Slowly move away from switch until switch is open
  while (!digitalRead(buttonPin2)) {
    Serial.println(steps[1]);
    digitalWrite(dirPinM1, HIGH);
    digitalWrite(stepPinM1, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(stepPinM1, LOW);
    delayMicroseconds(motorSpeed);
  }
  while(1){
    digitalWrite(stepPinM1, HIGH);
  }
}

void calibrateHomeMinMax(){

}