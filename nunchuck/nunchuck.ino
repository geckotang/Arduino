#include <MicroView.h>
#include <Wire.h>
#include "nunchuck.h"

int LED = A3;
char inputchar;
boolean LED_FLAG = false;

WiiChuck chuck = WiiChuck();
int angleStart, currentAngle;
int tillerStart = 0;
double angle;

void setup(){
  pinMode(LED, OUTPUT);

  chuck.begin();
  chuck.update();
  chuck.calibrateJoy();

  uView.setFontType(0);
  uView.begin();
  uView.clear(PAGE);

  Serial.begin(9600);
}

void loop(){
  delay(50);
  chuck.update();

  uView.clear(PAGE);
  uView.setCursor(0,0);

  uView.setCursor(0,0);
  uView.print("C:");
  uView.print(chuck.cPressed());
  uView.print(" X:");
  uView.print(chuck.readJoyX());
  uView.setCursor(0,10);
  uView.print("Z:");
  uView.print(chuck.zPressed());
  uView.print(" Y:");
  uView.print(chuck.readJoyY());
  uView.setCursor(0,20);
  uView.print("AX:");
  uView.print(chuck.readAccelX());
  uView.setCursor(0,30);
  uView.print("AY:");
  uView.print(chuck.readAccelY());
  uView.setCursor(0,40);
  uView.print("AZ:");
  uView.print(chuck.readAccelX());

  Serial.print("{");
  Serial.print("\"x\":");
  Serial.print(chuck.readJoyX());
  Serial.print(",\"y\":");
  Serial.print(chuck.readJoyY());
  Serial.print(",\"ax\":");
  Serial.print(chuck.readAccelX());
  Serial.print(",\"ay\":");
  Serial.print(chuck.readAccelY());
  Serial.print(",\"az\":");
  Serial.print(chuck.readAccelZ());
  Serial.println("}");

  if (chuck.cPressed() > 0 && chuck.zPressed() > 0) {
    Serial.println("{\"C\":true, \"Z\":true}");
  } else if (chuck.cPressed() > 0) {
    Serial.println("{\"C\":true, \"Z\":false}");
  } else if (chuck.zPressed() > 0) {
    Serial.println("{\"C\":false, \"Z\":true}");
  }

  uView.display();
}
