#define pulPinMotor1 8
#define dirPinMotor1 7
#define pulPinMotor2 13
#define dirPinMotor2 4
//#define stepsPerRev 800
#define stepperSpeed 500
#define FOR 1
#define BAC 0
float Gap_Width;
int Num_Fingers;
int Curr_Finger;
char myData[30] = {0};
float stepsPerRev;

void stepperRun(int steps, int direction, int pulPin, int dirPin);

void setup() {
  pinMode(pulPinMotor1, OUTPUT);    // set pin as PUL
  pinMode(dirPinMotor1, OUTPUT);    // set pin as DIR
  pinMode(pulPinMotor2, OUTPUT);    // set pin as PUL
  pinMode(dirPinMotor2, OUTPUT);    // set pin as DIR
  Serial.begin(9600);
  Serial.flush();
}

void loop() {
  
  byte n = Serial.available();
  if (n != 0){
    byte m = Serial.readBytesUntil('\n', myData, 30);
    myData[m] = '\0'; //null-byte
 
    Gap_Width = atof(strtok(myData, ","));
    stepsPerRev = (Gap_Width * 800);
    //Serial.print("Gap_Width = "); Serial.println(Gap_Width, 2);

    Num_Fingers = atoi(strtok(NULL, ","));
    //Serial.print("Num_Fingers = "); Serial.println(Num_Fingers);
    delay(1000);
    Serial.end();     }
    //ResetHoriz(1000);
  for(Curr_Finger = 0; Curr_Finger < Num_Fingers; Curr_Finger++){     
      delay(500);

      stepperRun(stepsPerRev, BAC, pulPinMotor1, dirPinMotor1);
      stepperRun(stepsPerRev/2, BAC, pulPinMotor2, dirPinMotor2);
      stepperRun(stepsPerRev/2, FOR, pulPinMotor2, dirPinMotor2);
      //Serial.print("Finger: #"); Serial.println(Curr_Finger+1);
      delay(1000);
      if ((Curr_Finger+1) == Num_Fingers){
        //Serial.print("Measurements Done");
        delay(1000);
        exit(0);


        
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

