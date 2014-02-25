void setup(){
  Serial.begin(9600);
  Serial.println("setupDone"); 
}

void loop(){
  Serial.println("loop");
  delay(500);
}
