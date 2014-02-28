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


String getCurrentMode(String _currentModeID){
  String str = "";
  if (_currentModeID == "A") {
      str = "favorite";
  }
  if (_currentModeID == "B") {
      str = "unfavorite";
  }
  if (_currentModeID == "C") {
      str = "mention";
  }
  return str;
}

void setup(){
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.println("setupDone"); 
}

void loop(){
  if (Serial.available() > 0) {
    inByte = Serial.read();
    // アカウント名を1文字ずつ受信するので
    // 受信中はアカウント名を連結する
    if(
        (inByte >= 65 && inByte <= 90) || (inByte >= 97 && inByte <= 122) 
        || (inByte >= 48 && inByte <= 57) || (inByte == 43)
        || (inByte == 61) || (inByte == 63)
    ){
      inMsg.concat(inByte);    
    }

    // 受信し終わったらしゃべる
    if (inByte == 10 || inByte == 13){
      inByte = 0;
      // inMsgの先頭の文字列を取り出して、favoriteかunfavoriteか判断する
      currentModeID = String(inMsg[0]);
      currentMode = getCurrentMode(currentModeID);
      targetName = inMsg.substring(1);
      
      // なければ処理しない
      if (currentModeID == "") {
        Serial.println("currentModeID is not found");
        return;
      }

      digitalWrite(LED, HIGH);
      while(atp.IsBusy());
      delay(2500);

      Serial.println("currentMode: " + currentMode);
      Serial.println("target name: " + targetName);
 
      if (targetName) {
        atp.SyntheS(targetName);
        inMsg = "";
        if (atp.GetResp() == 0) {
          atp.Synthe("sanga' ");
        }
      }

      if (currentMode == "favorite") {
          atp.Synthe("okiniirini/irema'_shita.");
      } else if (currentMode == "unfavorite") {
          atp.Synthe("okiniirio/hazushima'_shita.");
      } else if (currentMode == "mention") {
          atp.Synthe("hennshinn/_shitekima'_shita.");
      }

    }
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, LOW);
  }
}

