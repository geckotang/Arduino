/*
	SampleTalk - AquesTalkライブラリの色々な使い方サンプル

	This example code is in the public domain.
	14 Oct. 2012	N.Yamazaki (AQUEST)	Created
*/

#include <AquesTalk.h>
#include <Wire.h>  // I2Cライブラリ 必須！

AquesTalk atp;	// 音声合成のインスタンス

void setup()
{
	Serial.begin(9600);
  	Serial.println("-- setup() --");

	////////////////////////////////////////
	// AquesTalk pico LSIの応答チェック
	// LSIが応答するか最初にチェックしておくと良い
	////////////////////////////////////////
	if( !atp.IsActive() ){
		// 音声合成LSIから応答が得られない。配線等をチェック
		Serial.println("No response.");
	}
	else {
		Serial.println("Active");
	}
}

void loop()
{
  	Serial.println("-- loop() --");

	////////////////////////////////////////
	// 基本の発声
	////////////////////////////////////////
	Serial.print("Talking...");
	atp.Synthe("konnnichiwa.");  // 引数に指定のテキストを発声
	atp.SyntheP(PSTR("puroguramu/me'mori-de_su."));	// プログラム領域の文字列（RAMを節約）

	////////////////////////////////////////
	// 発声終了まで待つ
	////////////////////////////////////////
	while(atp.IsBusy()) ;	// Busyを返す間ループ
	Serial.println("End");

	delay(1000);

	////////////////////////////////////////
	// チャイム音
	////////////////////////////////////////
	Serial.println("Chime");
	atp.Synthe("#J");	// チャイム１
	atp.Synthe("#K");	// チャイム２

	while(atp.IsBusy()) ;  // wait end

	////////////////////////////////////////
	// 発話速度の変更
	////////////////////////////////////////
	Serial.println("SetSpeed SLOW");
	atp.SetSpeed(50);	// 発話速度を50にセット
	atp.SyntheP(PSTR("yukkuri."));

	while(atp.IsBusy()) ;  // wait end

	Serial.println("SetSpeed FAST");
	atp.SetSpeed(200);	// 発話速度を200にセット
	atp.SyntheP(PSTR("nama'mugi/nama'gome/namata'mago."));

	////////////////////////////////////////
	// 現在の発話速度を取得
	////////////////////////////////////////
	int speed = atp.GetSpeed();
	Serial.print("GetSpeed=");
	Serial.println(speed, DEC);

	////////////////////////////////////////
	// 発話速度をデフォルトに戻す
	////////////////////////////////////////
	atp.SetSpeed(100);	// 発話速度を100にセット

	////////////////////////////////////////
	// バージョン(AquesTalk pico LSI)取得
	////////////////////////////////////////
	char strVer[6];	// 6byte以上の配列を用意
	atp.GetVersion(strVer);	// バージョン文字列がstrVerに返される
	Serial.print("GetVersion=");
	Serial.println(strVer);

	delay(1000);
	
	////////////////////////////////////////
	// 発声を途中で中止する
	////////////////////////////////////////
	Serial.print("Break Sample  Talking...");
	atp.SyntheP(PSTR("tocyu-de/hasse-o/tomeru'niwa."));
	delay(1000);	// 1秒待ち
	atp.Break();	// BREAKコマンドの送信
	Serial.println("Break!");

	delay(1000);

	long num;
	String str;
	////////////////////////////////////////
	// 数値の発声(棒読み) NUMタグ
	// 記述方法の詳細は、AquesTalk pico LSI データシートの「音声記号列仕様」を参照
	////////////////////////////////////////
	num = 12345678;
	str = "bo-yomi <NUM VAL="+ String(num) + ">de_su.";
	Serial.println(str);
	atp.SyntheS(str);

	////////////////////////////////////////
	// 数値の発声(桁読み) NUMKタグ
	// 記述方法の詳細は、AquesTalk pico LSI データシートの「音声記号列仕様」を参照
	////////////////////////////////////////
	num = 987654321;
	str = "ketayomi <NUMK VAL=" + String(num) + ">de_su.";
	Serial.println(str);
	atp.SyntheS(str);

	while(atp.IsBusy()) ;  // wait end
	delay(1000);

	////////////////////////////////////////
	// 日時の発声 NUMKタグ(助数詞指定あり)
	// COUNTERで助数詞を指定することにより、読みやアクセントを適切に読ませることが可能
	//	例）20日："はつか"	30分："さんじゅっぷん"
	//	指定可能な助数詞は、AquesTalk pico LSI データシートの「音声記号列仕様」対応助数詞一覧表を参照
	////////////////////////////////////////
	int yy = 2012;
	int mm = 11;
	int dd = 20;
	str = "<NUMK VAL="+String(yy)+" COUNTER=nenn>";
	str += "/<NUMK VAL="+String(mm)+" COUNTER=gatu>";
	str += "/<NUMK VAL="+String(dd)+" COUNTER=nichi>";
	Serial.println(str);
	atp.SyntheS(str);

	while(atp.IsBusy()) ;  // wait end

	int hour = 23;
	int min = 30;
	int sec = 11;
	str = "<NUMK VAL="+String(hour)+" COUNTER=ji>";
	str += "/<NUMK VAL="+String(min)+" COUNTER=funn>";
	str += " <NUMK VAL="+String(sec)+" COUNTER=byo->";
	Serial.println(str);
	atp.SyntheS(str);



	while(atp.IsBusy()) ;  // wait end
	delay(5000);  // wait 5sec
}

