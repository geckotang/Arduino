int PIN_LED = 13;
int PIN_BTN_A = 8;
int buttonA = 0;
int lastButtonA = 0;
unsigned long time1 = 0;
int HOLDTIME = 300;

void setup() {
  Serial.begin(9600);
  Serial.println("-- setup() --");
  pinMode(PIN_BTN_A, INPUT) ;    //スイッチに接続ピンをデジタル入力に設定
  pinMode(13, OUTPUT) ;  //ＬＥＤに接続ピンをデジタル出力に設定
}

void loop() {
  buttonA = digitalRead(PIN_BTN_A);
  //ボタンの状態が変化
  if(buttonA != lastButtonA) {
    if(buttonA == HIGH) {
      //ボタンが押された時間を保存
      time1 = millis();
    }
    //ボタンの状態を保存
    lastButtonA = buttonA;
  } else {
    //ボタンが押されてから1秒経過し、かつボタンの状態がHIGH
    if((millis() - time1 > HOLDTIME) && (buttonA == HIGH)){
        time1 = 0;
	digitalWrite(PIN_LED, HIGH);
    } else {
	digitalWrite(PIN_LED, LOW);
    }
  }  
}

