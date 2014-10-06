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
  uView.print(chuck.buttonC);
  //uView.print(chuck.cPressed());
  uView.print(" X:");
  uView.print(chuck.readJoyX());
  uView.setCursor(0,10);
  uView.print("Z:");
  uView.print(chuck.buttonZ);
  //uView.print(chuck.zPressed());
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
  Serial.print(",\"button\":{");
  //押された時
  if (chuck.cPressed() && chuck.zPressed()) {
    Serial.print("\"press_c\":true, \"press_z\":true,");
  } else if (chuck.cPressed()) {
    Serial.print("\"press_c\":true, \"press_z\":false,");
  } else if (chuck.zPressed()) {
    Serial.print("\"press_c\":false, \"press_z\":true,");
  } else {
    Serial.print("\"press_c\":false, \"press_z\":false,");
  }
  //押し続けられている時
  if (chuck.buttonC && chuck.buttonZ) {
    Serial.print("\"hold_c\":true, \"hold_z\":true");
  } else if (chuck.buttonC) {
    Serial.print("\"hold_c\":true, \"hold_z\":false");
  } else if (chuck.buttonZ) {
    Serial.print("\"hold_c\":false, \"hold_z\":true");
  } else {
    Serial.print("\"hold_c\":false, \"hold_z\":false");
  }
  Serial.print("}");
  Serial.println("}");

  uView.display();
}
