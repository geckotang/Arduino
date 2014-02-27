/*
	PitchAccent - 声の高さ、アクセントの強さを変更するサンプル
				!! ATP3011R4 ONLY !!

	声の高さとともに、発話速度を変えるのも良い！
	
	This example code is in the public domain.
	14 Oct. 2012	N.Yamazaki (AQUEST)	Created
*/

#include <AquesTalk.h>
#include <Wire.h>  // I2Cライブラリ 必須！

AquesTalk atp;	// 音声合成のインスタンス

prog_char TestMessage[] PROGMEM =  "ko-doba'nngo-wa <ALPHA VAL=XPS3010>de_su.";

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	String str;

	atp.SetAccent(0x40);
	atp.SetPitch(0x40);
//	atp.SetSpeed(100);
	str = "Accent:"+String(atp.GetAccent())+" Pitch:"+String(atp.GetPitch());
	Serial.println(str);
	atp.SyntheP(TestMessage);
	while(atp.IsBusy()) ;  // wait end
	delay(1000);

	atp.SetAccent(0x20);
	atp.SetPitch(0x01);
//	atp.SetSpeed(120);
	str = "Accent:"+String(atp.GetAccent())+" Pitch:"+String(atp.GetPitch());
	Serial.println(str);
	atp.SyntheP(TestMessage);
	while(atp.IsBusy()) ;  // wait end
	delay(1000);

	atp.SetAccent(0x00);
	atp.SetPitch(0x20);
//	atp.SetSpeed(110);
	str = "Accent:"+String(atp.GetAccent())+" Pitch:"+String(atp.GetPitch());
	Serial.println(str);
	atp.SyntheP(TestMessage);
	while(atp.IsBusy()) ;  // wait end
	delay(1000);

	atp.SetAccent(0x40);
	atp.SetPitch(0x80);
//	atp.SetSpeed(80);
	str = "Accent:"+String(atp.GetAccent())+" Pitch:"+String(atp.GetPitch());
	Serial.println(str);
	atp.SyntheP(TestMessage);
	while(atp.IsBusy()) ;  // wait end
	delay(1000);

	// 出荷時設定に戻す
	atp.SetAccent(0xff);
	atp.SetPitch(0xff);
//	atp.SetSpeed(100);
	str = "Accent:"+String(atp.GetAccent())+" Pitch:"+String(atp.GetPitch());
	Serial.println(str);
	atp.SyntheP(TestMessage);
	while(atp.IsBusy()) ;  // wait end
	delay(1000);


	Serial.println("loop END");
	delay(5000);

}

