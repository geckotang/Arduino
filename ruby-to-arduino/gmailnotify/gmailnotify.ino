char inByte;
const int LED = 13;

void setup(){
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.println("setupDone"); 
}

void loop(){
  if (Serial.available() > 0) {
    inByte = Serial.read();
    Serial.println(inByte);
    digitalWrite(LED, HIGH);
    delay(500);
  } else {
    digitalWrite(LED, LOW);
  }
}
