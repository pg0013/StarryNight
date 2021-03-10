/**
 * @file	AppFrame\Script\Utility\Utility.h.
 *
 * @brief	Declares the utility class
 */

#include<string>
#include<Windows.h>
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

/**
 * @brief  画像、モデルで使用するハンドル
 */
typedef int handle;

namespace utility
{
	/**
	 * @brief ファイルの拡張子を取得
	 *
	 * @param _filename ファイル名
	 * @return 拡張子(.xxx)
	 */
	std::string GetExtension(std::string _filename);

	/**
	 * @brief　 プログラム終了時にメモリリークを検出する
	 */
	void CheckMemoryLeak();

	/**
	 * @brief	長さ_lengthの3d座標軸を描画.
	 *
	 * @param _length 座標軸の長さ
	 * @param _origin  座標軸の原点
	 */
	void Draw3DAxis(float _length, VECTOR _origin);

	/**
	 * @brief 3Dモデルのデバッグ情報の読み込み
	 *
	 * @param _handle モデルハンドル
	 * @param _name　表示するモデル名
	 * @param _draw_start_line 画面左に表示を開始する行番号
	 */
	void DrawModelDebugInfo(const int _handle, const std::string _name, const int _draw_start_line);

	/**
	 * @brief　処理を中断してエラーアラートを表示する
	 *
	 * @param エラー文面.
	 */
	void DrawDebugError(const std::string _error);

	/**
	 * @brief　Forwardベクトルを求める.
	 *
	 * @param _rot_y	求めるベクトルのy軸回転量[rad]
	 */
	VECTOR GetForwardVector(float& _rot_y);

	/**
	 * @brief　Rightベクトルを求める.
	 *
	 * @param _rot_y	求めるベクトルのy軸回転量[rad]
	 */
	VECTOR GetRightVector(float& _rot_y);

	/**
	 * @brief　Upベクトルを求める.
	 *
	 * @param _rot_y	求めるベクトルのy軸回転量[rad]
	 */
	VECTOR GetUpVector(float& _rot_y);

	/**
	 * @brief 左スティックの移動量を返す
	 *
	 * @return 左スティック移動量
	 */
	float GetLeftStickLength();

	/**
	 * @brief 左スティックの角度を返す
	 *
	 * @return 左スティックの角度
	 */
	float GetLeftStickRad();

	/**
	 * @brief ワールド座標をスクリーン座標に変換する
	 *				画面幅を超えていたら、画面幅の値を返す.
	 */
	VECTOR GetScreenPosFromWorldPos(VECTOR& _world_pos);

	/**
	 * @brief　クランプ関数.
	 */
	template<typename T>
	T clamp(T _value, T _min, T _max);
}