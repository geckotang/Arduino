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
  delay(1);
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
  Serial.print(",\"joyDeg\":");
  Serial.print(getFrontAngle( chuck.readJoyX(), chuck.readJoyY() ));
  Serial.print(",\"roll\":");
  Serial.print(chuck.readRoll());
  Serial.print(",\"pitch\":");
  Serial.print(chuck.readPitch());
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

int getFrontAngle (int joyX, int joyY) {
  boolean isLeft  = false;
  boolean isRight = false;
  boolean isTop  = false;
  boolean isBottom = false;
  int joyDeg = 999;

  //joyX
  if ( joyX < 100 ) {
    isLeft = true;
  } else if ( joyX > 140 ) {
    isRight = true;
  }
  //joyY
  if ( joyY > 0 ) {
    isTop = true;
  } else if ( joyY < -66 ) {
    isBottom = true;
  }

  if ( isTop && !isLeft && !isRight ) {
    //top
    joyDeg = 0;
  } else if ( isTop && isLeft && !isRight) {
    //top & left
    joyDeg = 45;
  } else if ( isTop && isRight && !isLeft ) {
    //top & right
    joyDeg = 315;
  } else if ( !isTop && !isBottom && isLeft) {
    //left
    joyDeg = 90;
  } else if ( !isTop && !isBottom && isRight) {
    //right
    joyDeg = 270;
  } else if ( !isTop && isBottom && isLeft) {
    //bottom & left
    joyDeg = 135;
  } else if ( !isTop && isBottom && isRight) {
    //bottom & right
    joyDeg = 225;
  } else if ( isBottom && !isLeft && !isRight) {
    //bottom
    joyDeg = 180;
  } else {
    // center
    joyDeg = 999;
  }

  return joyDeg;
}
