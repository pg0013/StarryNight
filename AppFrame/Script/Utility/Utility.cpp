/**
 * @file    Utility.cpp
 * @brief  便利系関数のまとめ
 *
 * @author Takuya Fujisawa
 * @date    20201021
 */
#include"Utility.h"
#include"DefineMacro.h"

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

	void DrawModelDebugInfo(const int _handle, const int _draw_start_line)
	{
		VECTOR position, rotation, scale;
		position = MV1GetPosition(_handle);
		rotation = MV1GetRotationXYZ(_handle);
		scale = MV1GetScale(_handle);

		int x, y;
		x = 0; y = _draw_start_line*DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "ModelInfo"); y += DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "  |- position : (%5.2f, %5.2f, %5.2f)", position.x, position.y, position.z); y += DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "  |- rotation : (%5.2f, %5.2f, %5.2f)", RAD2DEG(rotation.x), RAD2DEG(rotation.y), RAD2DEG(rotation.z)); y += DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "  -- scale     : (%5.2f, %5.2f, %5.2f)", scale.x, scale.y, scale.z); y += DEBUG_FONT_SIZE;
	}
}