int x;

#define buttonPin1 6  // the number of the pushbutton 
#define buttonPin2 5

int motorSpeed = 100;
int motorDistance = 4;
// variables will change:




void setup() {
  pinMode(9, OUTPUT);  // set Pin9 as PUL
  pinMode(8, OUTPUT);  // set Pin8 as DIR
  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT);
}
void loop() {

if(!digitalRead(buttonPin1)){
  digitalWrite(8, LOW);  // set high level direction
  for (x = 0; x < motorDistance; x++) {
    digitalWrite(9, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(9, LOW);
    delayMicroseconds(motorSpeed);
  }
}

if(digitalRead(buttonPin2)){
  digitalWrite(8, HIGH);     // set high level direction
  for (x = 0; x < motorDistance; x++) {
    digitalWrite(9, HIGH);   // Output high
    delayMicroseconds(motorSpeed);  // set rotate speed
    digitalWrite(9, LOW);    // Output low
    delayMicroseconds(motorSpeed);  // set rotate speed
  }
}

}