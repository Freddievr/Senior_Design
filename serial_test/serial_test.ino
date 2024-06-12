void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    String msg = Serial.readString();
  
    if (msg == "ON"){
      Serial.print("ON");
    }
    else if (msg == "OFF"){
      Serial.print("OFF");
    } 

    else{
      Serial.print("TRY AGAIN");
    }
  
  
  
  }
}