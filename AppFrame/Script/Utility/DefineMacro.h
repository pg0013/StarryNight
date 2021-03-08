#pragma once
#include"DxLib.h"

#ifdef _DEBUG
//#define DEBUG_FUNCTION
#endif // _DEBUG


#define DEBUG_COLOR GetColor(255,0,0)
#define DEBUG_FONT_SIZE (20)

//計算用マクロ
#define	PI	(3.1415926535897932386f)
#define DEG2RAD(x)		( ((x) / 180.0f)* PI)
#define RAD2DEG(x)		( ((x) * 180.0f ) / PI )

//XINPUT最大値
#define TRIGGER_MAX (255.0f)
#define THUMB_MAX (32767.0f)

//アナログスティックのデッドスペース
#define ANALOG_MIN (0.2f)
