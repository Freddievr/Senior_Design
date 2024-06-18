void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){

    String msg = Serial.readStringUntil('\r');




    if (msg == "SEND"){ 
      digitalWrite(LED_BUILTIN,HIGH);
      Serial.print("ON");
      Serial.print(msg);
    }

    else{
      Serial.print("TRY AGAIN");
      digitalWrite(LED_BUILTIN,LOW);
    }
  
  
  
  }
}