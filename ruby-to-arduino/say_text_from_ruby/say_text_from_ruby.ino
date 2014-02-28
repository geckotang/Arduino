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
  Serial.println("setupDone"); 
}

void loop(){
  if (Serial.available() > 0) {
    inByte = Serial.read();
    if(
        (inByte >= 65 && inByte <= 90) || (inByte >= 97 && inByte <= 122) 
        || (inByte >= 48 && inByte <= 57) || (inByte == 43)
        || (inByte == 61) || (inByte == 63)
    ){
      inMsg.concat(inByte);    
    }
    // 受信し終わったらしゃべる
    if (inByte == 10 || inByte == 13){
      inByte = 0;192.168.100.64
      //点灯
      digitalWrite(LED, HIGH);
      while(atp.IsBusy());
      delay(5000);
      atp.SyntheS(inMsg);
      Serial.println(inMsg);
      inMsg = "";
    }
    //消灯
    digitalWrite(LED, LOW);
  } else {
    //消灯
    digitalWrite(LED, LOW);
  }
}
