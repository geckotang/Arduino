// @link http://s-azcat.com/archives/389

int buttonA = 0;
int lastButtonA = 0;

int buttonB = 0;
int lastButtonB = 0;

unsigned long time1 = 0;
unsigned long time2 = 0;

int PIN_FLAG_A = 11;
int PIN_FLAG_B = 12;

int PIN_BTN_A = 8;
int PIN_BTN_B = 7;

int HOLDTIME = 4000;

boolean flagA = false;
boolean flagB = false;

void blinkLED() {
	if (!flagA && buttonA == HIGH) {
		digitalWrite(PIN_FLAG_A, HIGH);
		delay(500);
		digitalWrite(PIN_FLAG_A, LOW);
 		delay(500);
	}
	if (!flagB && buttonB == HIGH) {
		digitalWrite(PIN_FLAG_B, HIGH);
		delay(500);
      digitalWrite(PIN_FLAG_B, LOW);
      delay(500);
    }
}

void toggleLed() {
  
  blinkLED();
  // flagAがとれていれば
  if (flagA && !flagB) {
	  	digitalWrite(PIN_FLAG_A, HIGH);
	  	digitalWrite(PIN_FLAG_B, LOW);
  }
  // flagBがとれていれば
  if (flagB && !flagA) {
	  	digitalWrite(PIN_FLAG_A, LOW);
	  	digitalWrite(PIN_FLAG_B, HIGH);
  }
  // flagAもBもとれてなければ
  if (!flagA && !flagB) {
	  	digitalWrite(PIN_FLAG_A, LOW);
	  	digitalWrite(PIN_FLAG_B, LOW);
  }
}

void setup() {
  pinMode(PIN_BTN_A,INPUT);
  pinMode(PIN_BTN_B,INPUT);
  pinMode(PIN_FLAG_A, OUTPUT);
  pinMode(PIN_FLAG_B, OUTPUT);
}

void loop() {

  toggleLed();

  buttonA = digitalRead(PIN_BTN_A);
  buttonB = digitalRead(PIN_BTN_B);

  //
  // buttnA (RED)
  //

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
        flagA = true;
        flagB = false;
        time1 = 0;
    }
  }

  //
  // buttnB (BLUE)
  //
  if(buttonB != lastButtonB) {
    if(buttonB == HIGH) {
      time2 = millis();
    }
    lastButtonB = buttonB;
  } else {
    if((millis() - time2 > HOLDTIME) && (buttonB == HIGH)){
        flagA = false;
        flagB = true;
        time2 = 0;
    }
  }


}

