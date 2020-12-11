/**
 * @file	AppFrame\Script\Utility\Utility.h.
 *
 * @brief	Declares the utility class
 */

#include<string>
#include"DxLib.h"

/**
 * @brief メモリリークチェック用.
 */
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define NEW new
#endif

typedef int handle;

namespace utility
{
	/**
	 * @brief ファイルの拡張子を取得
	 *
	 * @param ファイル名
	 * @return 拡張子(.xxx)
	 */
	std::string GetExtension(std::string _filename);

	/**
	 * @brief　 プログラム終了時にメモリリークを検出する
	 */
	void CheckMemoryLeak();

	/**
	 * @brief長さ_lengthの3d座標軸を描画.
	 */
	void Draw3DAxis(float _length, VECTOR _vector);
}