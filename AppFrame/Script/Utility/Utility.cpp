/**
 * @file    Utility.cpp
 * @brief  便利系関数のまとめ
 *
 * @author Takuya Fujisawa
 * @date    20201021
 */
#include"Utility.h"

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
		DrawLine3D(VAdd(_origin, axis_z), VAdd(_origin, VScale(axis_z,-1.0f)), GetColor(0, 0, 255));
	}
}