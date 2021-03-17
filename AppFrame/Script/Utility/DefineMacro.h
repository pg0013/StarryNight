#pragma once
#include"DxLib.h"

#ifdef _DEBUG
#define DEBUG_FUNCTION
#endif // _DEBUG

#define DEBUG_COLOR  = GetColor(255, 0, 0);
constexpr int  DEBUG_FONT_SIZE = 20;

//計算用マクロ
constexpr float	PI = 3.1415926535897932386f;
template<typename T>
constexpr float DEG2RAD(T x){ return (((x) / 180.0f) * PI); }

template<typename T>
constexpr float  RAD2DEG(T x) { return (((x) * 180.0f) / PI); }

//XINPUT最大値
constexpr float  TRIGGER_MAX = (255.0f);
constexpr float  THUMB_MAX = (32767.0f);

//パッド入力のトリガー用
constexpr int  PAD_INPUT_ALL = (0xffff);

//アナログスティックのデッドスペース
constexpr float  ANALOG_MIN = (0.2f);