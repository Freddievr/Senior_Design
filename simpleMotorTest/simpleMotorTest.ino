int x;
int motorDistance = 4000;
int motorSpeed = 300;
void setup() {
  pinMode(9, OUTPUT);  // set Pin9 as PUL
  pinMode(8, OUTPUT);  // set Pin8 as DIR
}
void loop() {
  digitalWrite(8, HIGH);     // set high level direction
  for (x = 0; x < motorDistance; x++)  // repeat 400 times a revolution when setting 400 on driver
  {
    digitalWrite(9, HIGH);   // Output high
    delayMicroseconds(motorSpeed);  // set rotate speed
    digitalWrite(9, LOW);    // Output low
    delayMicroseconds(motorSpeed);  // set rotate speed
  }
  delay(1000);           //pause 1 second
  digitalWrite(8, LOW);  // set high level direction
  for (x = 0; x < motorDistance; x++) {
    digitalWrite(9, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(9, LOW);
    delayMicroseconds(motorSpeed);
  }
  delay(1000);
}