#include <AquesTalk.h>
#include <Wire.h>  // I2CライブラリAquesTalkライブラリ内部で使用するので定義必要

// Hello Talk - AquesTalk pico LSIで「こんにちは」を繰り返し発声する

AquesTalk atp;  //インスタンス定義 変数名は任意

void setup()
{
}

void loop()
{
	atp.Synthe("konnnichiwa.");  //引数に指定のテキストを発声
}

