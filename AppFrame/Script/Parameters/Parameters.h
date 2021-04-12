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
#include"../Utility/Utility.h"
#include "DxLib.h"

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
	 * @brief 3Dモデル用情報格納構造体
	 */
	struct model_param
	{
		std::string filename_;
		std::string handlename_;
		VECTOR position_;
		VECTOR rotation_;
		VECTOR scale_;
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
		 * @param  _folder_name ファイル名
		 * @param  _async_flag　非同期読み込み用フラグ
		 */
		void LoadImagParameters(const std::string& _file_name, const bool& _async_flag = false);

		/**
		 * @brief　 Resource/json/model.jsonにあるオブジェクトのモデルパラメータを読み込む処理
		 *
		 * @param  _object_name　	model.jsonで指定するオブジェクト名.Resource/model配下のフォルダ名と同じ必要がある
		 * @param  _async_flag			非同期読み込み処理
		 */
		void LoadModelParameters(const std::string& _object_name, const bool& _async_flag = false);

		std::unordered_map<std::string, imag_param> GetMapImageParam() const { return map_imag_param_; }
		std::unordered_map<std::string, model_param> GetMapModelParam() const { return map_model_param_; }

	protected:
		std::unordered_map<std::string, imag_param> map_imag_param_;
		std::unordered_map<std::string, model_param> map_model_param_;
	};
}