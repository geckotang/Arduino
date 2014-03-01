#include <AquesTalk.h>
#include <Wire.h>
AquesTalk atp;

boolean isCheckMode = true;
String currentMode;
String currentModeID;
String targetName;
String str;
String inMsg;
char inByte;
const int LED = 13;

int count = 0;
boolean flag;

void setup(){
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  if (Serial.available() > 0) {
    inByte = Serial.read();
    if(inByte != 10 || inByte != 13){
      inMsg.concat(inByte);
    }
    // 受信し終わったらしゃべる
    if (inByte == 10 || inByte == 13){
      inByte = 0;
      //点灯
      digitalWrite(LED, HIGH);
      while(atp.IsBusy());
      delay(5000);
      atp.SyntheS(inMsg);
      Serial.print("end");
      inMsg = "";
    }
    //消灯
    digitalWrite(LED, LOW);
  } else {
    //消灯
    digitalWrite(LED, LOW);
  }
}
