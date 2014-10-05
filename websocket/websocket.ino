#include <MicroView.h>

int LED = A3;
char inputchar;
boolean LED_FLAG = false;

void setup(){
  pinMode(LED, OUTPUT);

  uView.setFontType(0);
  uView.begin();
  uView.clear(PAGE);

  Serial.begin(9600);
}

void loop(){
  uView.clear(PAGE);
  uView.setCursor(0,0);

  if (Serial.available() > 0) {
    inputchar = Serial.read();
    if (inputchar == 'o') {
      LED_FLAG = true;
      Serial.println("LED:ON");
      digitalWrite(LED, HIGH);
    } else if (inputchar == 'x') {
      LED_FLAG = false;
      Serial.println("LED:OFF");
      digitalWrite(LED, LOW);
    }
  }
  
  uView.print("LED:");
  if (LED_FLAG) {
    uView.print("ON");
  } else {
    uView.print("OFF");
  }

  uView.display();
}
