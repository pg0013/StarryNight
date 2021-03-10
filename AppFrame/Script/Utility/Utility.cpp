/**
 * @file    Utility.cpp
 * @brief  便利系関数のまとめ
 *
 * @author Takuya Fujisawa
 * @date    20201021
 */
#include"Utility.h"
#include"DefineMacro.h"
#include"../Application/ApplicationBase.h"
#include<string>

namespace utility
{
	std::string GetExtension(std::string _filename)
	{
		//最後に.がある場所を取得
		int ext_pos = static_cast<int>(_filename.find_last_of("."));
		//ファイル名から拡張子を切り出す
		std::string ext_name = _filename.substr(ext_pos, _filename.size() - ext_pos);
		return ext_name;
	}

	void CheckMemoryLeak()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	void Draw3DAxis(float _length, VECTOR _origin)
	{
		VECTOR axis_x = VGet(_length, 0, 0);
		VECTOR axis_y = VGet(0, _length, 0);
		VECTOR axis_z = VGet(0, 0, _length);

		//軸を描画
		DrawLine3D(VAdd(_origin, axis_x), VAdd(_origin, VScale(axis_x, -1.0f)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(_origin, axis_y), VAdd(_origin, VScale(axis_y, -1.0f)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(_origin, axis_z), VAdd(_origin, VScale(axis_z, -1.0f)), GetColor(0, 0, 255));
	}

	void DrawModelDebugInfo(const int _handle, const std::string _name, const int _draw_start_line)
	{
		VECTOR position, rotation, scale;
		position = MV1GetPosition(_handle);
		rotation = MV1GetRotationXYZ(_handle);
		scale = MV1GetScale(_handle);

		int x, y;
		x = 0; y = _draw_start_line * DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "%2d ModelInfo : %s", y / DEBUG_FONT_SIZE, _name.c_str()); y += DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   |- position : (%5.2f, %5.2f, %5.2f)", y / DEBUG_FONT_SIZE, position.x, position.y, position.z); y += DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   |- rotation : (%5.2f, %5.2f, %5.2f)", y / DEBUG_FONT_SIZE, RAD2DEG(rotation.x), RAD2DEG(rotation.y), RAD2DEG(rotation.z)); y += DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- scale     : (%5.2f, %5.2f, %5.2f)", y / DEBUG_FONT_SIZE, scale.x, scale.y, scale.z); y += DEBUG_FONT_SIZE;
	}

	void DrawDebugError(const std::string _error)
	{
		_RPTF0(_CRT_ERROR, _T(_error.c_str()));
	}

	VECTOR GetForwardVector(float& _rot_y)
	{
		//モデルがz軸の負の方向を向いているため
		VECTOR forward = VGet(0, 0, -1);
		MATRIX rot_matrix = MGetRotY(_rot_y);

		//forwardベクトルを返す
		return VTransform(forward, rot_matrix);
	}

	VECTOR GetRightVector(float& _rot_y)
	{
		VECTOR forward = GetForwardVector(_rot_y);
		VECTOR up = VGet(0, 1, 0);//仮のupベクトル

		//rightベクトルを返す
		return VCross(up, forward);
	}

	VECTOR GetUpVector(float& _rot_y)
	{
		VECTOR forward = GetForwardVector(_rot_y);
		VECTOR right = GetRightVector(_rot_y);

		//upベクトルを返す
		return VCross(forward, right);
	}

	float GetLeftStickLength()
	{
		XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

		float stick_lx, stick_ly;//アナログスティックの座標

		stick_lx = x_input.ThumbLX / THUMB_MAX;
		stick_ly = -x_input.ThumbLY / THUMB_MAX;

		float length = static_cast<float>(sqrt(stick_lx * stick_lx + stick_ly * stick_ly));

		return length;
	}

	float GetLeftStickRad()
	{
		XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

		float stick_lx, stick_ly;//アナログスティックの座標

		stick_lx = x_input.ThumbLX / THUMB_MAX;
		stick_ly = -x_input.ThumbLY / THUMB_MAX;

		float rad = static_cast<float>(atan2(stick_lx, stick_ly));
		return rad;
	}

	VECTOR GetScreenPosFromWorldPos(VECTOR& _world_pos)
	{
		VECTOR screen_pos = ConvWorldPosToScreenPos(_world_pos);

		//画面サイズに値を制限する
		screen_pos.x = clamp(screen_pos.x, 0.0f, static_cast<float>(appframe::SCREEN_WIDTH));
		screen_pos.y = clamp(screen_pos.y, 0.0f, static_cast<float>(appframe::SCREEN_HEIGHT));

		return screen_pos;
	}

	template<typename T>
	T clamp(T _value, T _min, T _max)
	{
		return min(max(_value, _min), _max);
	}
}