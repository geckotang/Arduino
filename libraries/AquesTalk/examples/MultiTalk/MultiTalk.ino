/*
	MultiTalk - 複数の音声合成LSIを制御するサンプル

	AquesTalkシールドを複数重ねて、同時に複数の発声を制御することができる。

	それぞれのデバイスを区別するために、あらかじめI2Cアドレスを変えておく必要がある。
	具体的には、ATP3011のEEPROMのアドレス0x006の値を変更すればよい。
	変更方法としては、AquesTalk pico LSIをArduino Unoボードに挿してターミナルから#Wコマンドを実行。
	たとえば、このスケッチの場合I2Cアドレスを0x2Fとしているので、"#W0062F"を実行する。
	【注意】I2Cアドレスが不明になると動作できなくなるので、どこかにメモしておくこと。
			アドレスが不明な場合は、セーフモードで動作させる(データシート参照)


	This example code is in the public domain.
	14 Oct. 2012	N.Yamazaki (AQUEST)	Created
*/

#include <AquesTalk.h>
#include <Wire.h>  // I2Cライブラリ 必須！

AquesTalk atp1(AQTK_I2C_ADDR);	// デフォルトI2Cアドレス(0x2E)
AquesTalk atp2(0x2F);	// もうひとつのデバイスのI2Cアドレスは0x2Fとする

void setup()
{
	Serial.begin(9600);

	// AquesTalk pico LSIの応答チェック
	if( !atp1.IsActive() )	Serial.println("LSI1 No response.");
	else					Serial.println("LSI1 Ready.");
	if( !atp2.IsActive() )	Serial.println("LSI2 No response.");
	else					Serial.println("LSI2 Ready.");
}

void loop()
{
	///////////////////////////////////
  	// 交互に発話
	///////////////////////////////////
	atp1.SyntheP(PSTR("konnnichiwa-."));

	while(atp1.IsBusy()) ;  // wait end
	atp2.SyntheP(PSTR("konnnichiwa."));
	while(atp2.IsBusy()) ;  // wait end

	atp1.SyntheP(PSTR("yo'i/te'nnkidesune?"));
	while(atp1.IsBusy()) ;  // wait end

	atp2.SyntheP(PSTR("honnto yo'i/te'nnkidesune?"));
	while(atp2.IsBusy()) ;  // wait end

	delay(1500);
	
	///////////////////////////////////
  	// 同時に発話
	///////////////////////////////////
	atp1.SyntheP(PSTR("jyu'gemu/jyu'gemu go'ko-no/surikire,"));
	atp2.SyntheP(PSTR("tokoro'de,"));

	atp1.SyntheP(PSTR("kaijyari/su'igyono/suigyo'-matu,"));
	delay(1000);
	atp2.SyntheP(PSTR("ano-"));

	atp1.SyntheP(PSTR("unnra'imatu fu-ra'imatu ku'-nerutokoroni/su'mutokoro."));
	delay(700);
	atp2.SyntheP(PSTR("ano---?"));

	atp1.SyntheP(PSTR("yaburako'-jino/burako'-ji.pa'ipo/pai'pono/syu-ri'nngann,"));
	delay(700);
	atp2.SyntheP(PSTR("i-kagennni/_sitekudasa'i."));
	while(atp2.IsBusy()) ;  // wait end
	atp1.Break();	// 途中で発話を中断する

	delay(700);
	atp1.SyntheP(PSTR("+gomennnasa'i."));

	while(1) ;  // 無限ループ
}

