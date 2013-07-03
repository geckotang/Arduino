//4

const int LED = 9;
int i = 0;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  for (i=0; i<100; i++){
    analogWrite(LED, i);
    delay(10);
  }
  for (i=100; i>0; i--){
    analogWrite(LED, i);
    delay(10);
  }
}
