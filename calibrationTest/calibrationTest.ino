int x;
int motorDistance = 8000;
int motorSpeed = 300;
const int buttonPin1 = 6;  // the number of the pushbutton pin
const int buttonPin2 = 5;
int buttonState;

void setup() {
  pinMode(9, OUTPUT);  // set Pin9 as PUL
  pinMode(8, OUTPUT);  // set Pin8 as DIR
    // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
}
void loop() {
  while(buttonPress1()){
    digitalWrite(8, HIGH);     // set high level direction
    for (x = 0; x < motorDistance; x++)  // repeat 400 times a revolution when setting 400 on driver
    {
      digitalWrite(9, HIGH);   // Output high
      delayMicroseconds(motorSpeed);  // set rotate speed
      digitalWrite(9, LOW);    // Output low
      delayMicroseconds(motorSpeed);  // set rotate speed
    }
  }

  while(buttonPress2()){
    digitalWrite(8, LOW);     // set high level direction
    for (x = 0; x < motorDistance; x++)  // repeat 400 times a revolution when setting 400 on driver
    {
      digitalWrite(9, HIGH);   // Output high
      delayMicroseconds(motorSpeed);  // set rotate speed
      digitalWrite(9, LOW);    // Output low
      delayMicroseconds(motorSpeed);  // set rotate speed
    }
  }
}

bool buttonPress1() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin1);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // return true for button press
    return true;
  } 
  else {
    // reset to false
    return false;
  }
}

bool buttonPress2() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin2);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // return true for button press
    return true;
  } 
  else {
    // reset to false
    return false;
  }
}