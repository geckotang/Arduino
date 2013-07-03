//2

// LEDが接続されているピン
const int LED = 13;
// プッシュボタンが接続されているピン
const int BUTTON = 7;
// 入力ピンの状態
int val = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  val = digitalRead(BUTTON);
  if (val == HIGH) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}

