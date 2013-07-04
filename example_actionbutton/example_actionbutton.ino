/*
 * ActionButton example
 */
#include <ActionButton.h>

ActionButton btn1 = ActionButton(7, ActionButton::PULL_UP);

// ボタンが押されたら
void onPressed(ActionButton & btn) {
  Serial.print("button ");
  Serial.print(btn.getPin());
  Serial.println(" pressed"); 
}

// ボタンが離されたら
void onReleased(ActionButton & btn) {
  Serial.print("button ");
  Serial.print(btn.getPin());
  Serial.println(" released");   
}

// 長押しされたら?
void onSustainedPress(ActionButton & btn) {
  Serial.print("button ");
  Serial.print(btn.getPin());
  Serial.println(" ga naga oshi sareteimasu"); 
}

// 変化があったら
void onChange(ActionButton & btn) {
  String state = "";
  
  switch (btn.getState()) {
  	case ActionButton::PRESSED :
  	  state = " pressed";
  	  break;
  	case ActionButton::RELEASED :
  	  state = " released";
  	  break;
  }
  Serial.print("button ");
  Serial.print(btn.getPin());
  Serial.println(state);
}

void setup() {
  Serial.begin(57600);
  // 変化があった時に実行する
  btn1.attach(ActionButton::CHANGED, onChange);
  // 機能してない？？
  btn1.attach(ActionButton::SUSTAINED, onSustainedPress);  
}

void loop() {
  //監視する
  btn1.update();
}

