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
    /*
     32 : 空白
    '39 : クオート
    -45 : ハイフン
    .46 : ドット
    /47 : スラッシュ
    _95 : アンダースコア
    */        
    if(
//        inByte != 10 || inByte != 13
        (inByte >= 65 && inByte <= 90) || (inByte >= 95 && inByte <= 122) 
        || (inByte >= 32 && inByte <= 57) || (inByte == 43)
        || (inByte == 61) || (inByte == 63)
        || (inByte == 32)
        || (inByte == 60)
        || (inByte == 62)
    ){
      inMsg.concat(inByte);
    }
    // 受信し終わったらしゃべる
    if (inByte == 10 || inByte == 13){
      inByte = 0;
      //点灯
      digitalWrite(LED, HIGH);
      while(atp.IsBusy());
      delay(100);
      atp.SyntheS(inMsg);
      Serial.print('e');
      inMsg = "";
    }
    //消灯
    digitalWrite(LED, LOW);
  } else {
    //消灯
    digitalWrite(LED, LOW);
  }
}
