// Requrements
// ActionButton library by Jeff Hoefs
// https://github.com/soundanalogous/ActionButton/

#include <ActionButton.h>

const int ledPin_1 = 13; // RED
const int ledPin_2 = 12; // BLUE
const int btnPin_1 = 8;  // RED
const int btnPin_2 = 7;  // BLUE
const int pressedTime = 1000;
const int maxCount = 5;

int currentCount = 0;
int currentBtnPin = 0;
int currentLedPin = 0;

boolean isActiveLed_1 = false;
boolean isActiveLed_2 = false;

ActionButton btn_1 = ActionButton(btnPin_1, ActionButton::PULL_DOWN);
ActionButton btn_2 = ActionButton(btnPin_2, ActionButton::PULL_DOWN);

// ボタンが押されたら
void onPressed(ActionButton & btn) {
  // シリアルに状態をプリントし
  Serial.print("pressed ");
  Serial.print("button ");
  Serial.println(btn.getPin());

  // LEDを点灯
  if (btn.getPin() == btnPin_1) {
    digitalWrite(ledPin_1, HIGH);
  } else {
    digitalWrite(ledPin_2, HIGH);
  }
}


// ボタンが離されたら
void onReleased(ActionButton & btn) {
  // シリアルに状態をプリントし
  Serial.print("released ");
  Serial.print("button ");
  Serial.println(btn.getPin());

  currentCount = 0;

  // LEDを消灯
  if (btn.getPin() == btnPin_1 && currentBtnPin != btnPin_1) {
      digitalWrite(ledPin_1, LOW);
  } else if (currentBtnPin != btnPin_2 &&  currentBtnPin != btnPin_2) {
      digitalWrite(ledPin_2, LOW);    
  } else {
  }

}

// ボタンが長押しされたら
void onSustainedPress(ActionButton & btn) {
  // シリアルに状態をプリントし
  Serial.print("sustained ");
  Serial.print("button ");
  Serial.println(btn.getPin());

  // 現在長押しした方のボタン
  currentBtnPin = btn.getPin();

  // 既に点灯していたら実行しない
  if (
      (currentBtnPin == btnPin_1 && isActiveLed_1) ||
      (currentBtnPin == btnPin_2 && isActiveLed_2)
  ) {
    Serial.println("you already got this flag!");
    return;
  }

  currentCount++;
  
  if (currentCount < maxCount) {
    if (btn.getPin() == btnPin_1) {
      digitalWrite(ledPin_1, HIGH);
      delay(250);
      digitalWrite(ledPin_1, LOW);
      delay(250);
    }else{
      digitalWrite(ledPin_2, HIGH);
      delay(250);
      digitalWrite(ledPin_2, LOW);
      delay(250);
    }    
  } else {
    currentCount = 0;
    // 点灯させたままにするLED
    if (currentBtnPin == btnPin_1) {
      currentLedPin = ledPin_1;
      // 逆側のLEDを消灯
      digitalWrite(ledPin_2, LOW);
      isActiveLed_1 = true;
      isActiveLed_2 = false;
    } else {
      currentLedPin = ledPin_2;
      // 逆側のLEDを消灯
      digitalWrite(ledPin_1, LOW);
      isActiveLed_1 = false;
      isActiveLed_2 = true;
    }
  
    // 3回点滅
    for (int i = 0; i < 5; i++) {
      digitalWrite(currentLedPin, HIGH);
      delay(200);
      digitalWrite(currentLedPin, LOW);
      delay(200);
    }
    digitalWrite(currentLedPin, HIGH);
  }

}

void setup() {
  pinMode(ledPin_1, OUTPUT);
  pinMode(ledPin_2, OUTPUT);

  Serial.begin(9600);

  // ボタンにイベントをセット
  btn_1.attach(ActionButton::PRESSED, onPressed);
  btn_1.attach(ActionButton::RELEASED, onReleased);
  btn_1.attach(ActionButton::SUSTAINED, onSustainedPress);
  btn_1.setSustainedInterval(pressedTime);

  // ボタンにイベントをセット
  btn_2.attach(ActionButton::PRESSED, onPressed);
  btn_2.attach(ActionButton::RELEASED, onReleased);
  btn_2.attach(ActionButton::SUSTAINED, onSustainedPress);
  btn_2.setSustainedInterval(pressedTime);


}

void loop() {
  // ループ中で毎回updateメソッドを呼んで更新
  btn_1.update();
  btn_2.update();
}
