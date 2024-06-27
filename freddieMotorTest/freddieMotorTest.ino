#define pulPinMotor1 8
#define dirPinMotor1 7
#define pulPinMotor2 13
#define dirPinMotor2 4
//#define stepsPerRev 800
#define stepperSpeed 500
#define FOR 1
#define BAC 0
float gapWidth;
int numFingers;
int currFinger;
char myData[30] = {0};
int stepsPerRev;

void stepperRun(int steps, int direction, int pulPin, int dirPin);

void setup() {
  pinMode(pulPinMotor1, OUTPUT);    // set pin as PUL
  pinMode(dirPinMotor1, OUTPUT);    // set pin as DIR
  pinMode(pulPinMotor2, OUTPUT);    // set pin as PUL
  pinMode(dirPinMotor2, OUTPUT);    // set pin as DIR
  Serial.begin(9600);
  Serial.flush();
  currFinger = 0;
}

void loop() {
  
  byte n = Serial.available();
  if (n != 0){
    byte m = Serial.readBytesUntil('\n', myData, 30);
    myData[m] = '\0'; //null-byte
 
    gapWidth = atof(strtok(myData, ",")); // Separates string using "," as delimiter
    stepsPerRev = gapWidth;
    //Serial.print("gapWidth = "); Serial.println(gapWidth, 2);
    //Serial.print("Steps "); Serial.println(stepsPerRev);
    numFingers = atoi(strtok(NULL, ",")); // Converts ASCII number to integer
    //Serial.print("Num_Fingers = "); Serial.println(numFingers);
    delay(1000);
    Serial.end();     
  }
    //ResetHoriz(1000);
  do {   
    delay(500);
    stepperRun(stepsPerRev, BAC, pulPinMotor1, dirPinMotor1);
    stepperRun(stepsPerRev/2, BAC, pulPinMotor2, dirPinMotor2);
    stepperRun(stepsPerRev/2, FOR, pulPinMotor2, dirPinMotor2);
    //Serial.print("Finger: #"); Serial.println(Curr_Finger+1);
    //Serial.print("Steps"); Serial.println(stepsPerRev);
    delay(1000);
    currFinger++;
    // exit(0);      
  }
  while(currFinger != numFingers);
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

void ResetHoriz(int steps){  
    
  digitalWrite(dirPinMotor1, FOR);  // set direction level
  for (int x = 0; x < steps; x++)   // repeat "steps" times a revolution
  {
    digitalWrite(pulPinMotor1, HIGH);     // Output high
    delayMicroseconds(stepperSpeed);// set rotate speed
    digitalWrite(pulPinMotor1, LOW);      // Output low
    delayMicroseconds(stepperSpeed);// set rotate speed
  }
  delay(1000);                      // May be unnecessary
}

