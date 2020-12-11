/**
 * @file    Parameters.h
 * @brief  ゲーム内定数管理クラス
 *				画像情報はjsonから読み込む
 *
 * @author Takuya Fujisawa
 * @date   2020/11/02
 */

#pragma once
#include<string>
#include<unordered_map>

namespace parameters
{
	/**
 * @brief 画像情報格納用構造体
 */
	struct imag_param
	{
		std::string filename_;
		int xnum_;
		int ynum_;
		int xsize_;
		int ysize_;
	};

	/**
	 * @brief ゲーム内変数格納用クラス
	 */
	class Parameters
	{
	public:
		Parameters();
		~Parameters();

		/**
		 * @brief 画像パラメータをjsonから読み込み
		 *				パラメータはファイル名をkeyとしたimag_map_に保存
		 *
		 * @param  _mode　モード名
		 */
		void LoadImagParameters(std::string _mode,bool _async_flag = false);
	};
}