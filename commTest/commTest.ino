
void setup()
{
  Serial.begin(9600); 
  Serial.setTimeout(1);
  pinMode(LED_BUILTIN, OUTPUT); 

}
//--------------------------------------------------------------
void loop(){
  // char controlSignal;
  // while(!Serial.available());
  
  // controlSignal = Serial.read();
  // if(controlSignal == 'R' || 'B' || 'G') digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  String Message;

  while (!Serial.available());
  Message = Serial.readString();
  Message.toUpperCase();
  Serial.print(Message);

}