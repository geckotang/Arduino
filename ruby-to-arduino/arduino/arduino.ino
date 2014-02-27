#include <AquesTalk.h>
#include <Wire.h>
AquesTalk atp;

char inByte;
String str;
String inMsg;
const int LED = 13;

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
      digitalWrite(LED, HIGH);
      while(atp.IsBusy());
      delay(1000);
      inMsg.concat(".");
      inMsg = "jekkota'nngu";
      Serial.println(inMsg);
      atp.SyntheS(inMsg);
      Serial.println(atp.GetResp());
      //atp.Synthe("/ni/faborare/mashita.");
      inMsg = "";
    }
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, LOW);
  }
}
