//5

const int LED = 9;
const int BUTTON = 7;

int val = 0;
int old_val = 0;
boolean state = false;

int brightness = 128;
unsigned long startTime = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  val = digitalRead(BUTTON);

  // 変化があるかどうかチェック
  if ((val == HIGH) && (old_val == LOW)){
    state = !state;
    startTime = millis();
    delay(10);
  }
  
  // ボタンが押し続けられているのかをチェック
  if ((val == HIGH) && (old_val == HIGH)){
    // 500ms以上押されているか
    if (state == true && (millis() - startTime) > 500){
      brightness++;
      delay(10);
      if (brightness > 255) {
        brightness = 0;
      }
    }
  }
  
  old_val = val;
  
  if (state == true){
    analogWrite(LED, brightness);
  } else {
    analogWrite(LED, 0);
  }
    
}


