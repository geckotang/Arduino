#include <AquesTalk.h>
#include <Wire.h>  // I2Cライブラリ 必須！
AquesTalk atp; // 音声合成のインスタンス

void setup(){
	Serial.begin(9600);
  	Serial.println("-- setup() --");
	if( !atp.IsActive() ){
		Serial.println("No response.");
	}
	else {
		Serial.println("Active");
	}
}

void loop(){
        Serial.println(atp.IsBusy());
	Serial.println("-- loop() --");
	while(atp.IsBusy());
	delay(5000);
	Serial.println("End");

	atp.Synthe("#J");
	atp.Synthe("#K");
        atp.SyntheP(PSTR("kaedama/barikata/hutatsu."));
}

