#define xRS 13  // Pin 12 connected to Home Switch (MicroSwitch)
#define xLS 12
#define zTS 11  // Pin 12 connected to Home Switch (MicroSwitch)
#define zBS 10

int sensorVal1;
int sensorVal2; // Pin works
int sensorVal3;
int sensorVal4;

int currentVal1;
int currentVal2;
int currentVal3;
int currentVal4;

int prev1;
int prev2;
int prev3;
int prev4;

void setup() {
  //start serial connection
  Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(zBS, INPUT_PULLUP);
  pinMode(zTS, INPUT_PULLUP);
  pinMode(xLS, INPUT_PULLUP);
  pinMode(xRS, INPUT_PULLUP);

  prev1 = 1;
  prev2 = 1;
  prev3 = 1;
  prev4 = 1;
}

void loop() {
  //read the pushbutton value into a variable
  currentVal1 = digitalRead(xRS);
  currentVal2 = digitalRead(xLS); // Pin works
  currentVal3 = digitalRead(zTS);
  currentVal4 = digitalRead(zBS);

  sensorVal1 = currentVal1 + prev1;
  sensorVal2 = currentVal2 + prev2;
  sensorVal3 = currentVal3 + prev3;
  sensorVal4 = currentVal4 + prev4;

  prev1 = currentVal1;
  prev2 = currentVal2;
  prev3 = currentVal3;
  prev4 = currentVal4;
  
  //print out the value of the pushbutton
  
  Serial.print(sensorVal1);
  Serial.print("\t");
  Serial.print(sensorVal2);
  Serial.print("\t");
  Serial.print(sensorVal3);
  Serial.print("\t");
  Serial.println(sensorVal4);

  if (sensorVal1 == 0 || sensorVal2 == 0 || sensorVal3 == 0 || sensorVal4 == 0){
    while(1){
      int op = 0;
    }
  }


}
