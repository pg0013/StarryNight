/**
 * @file    CameraParameters.h
 * @brief  カメラパラメータ読み込みクラス
 *
 * @author Takuya Fujisawa
 * @date   2020/12/24
 */
#pragma once
#include"appframe.h"

namespace starrynight
{
	namespace camera
	{
		/**
		 * @brief カメラパラメータ読み込みクラス
		 */
		class CameraParameters :
			public parameters::Parameters
		{
		public:
			CameraParameters();
			~CameraParameters();

			/**
			 * @brief カメラパラメータをjsonファイルから読み込む
			 *
			 * @param  _param_name　パラメータ名を格納したコンテナ
			 */
			void LoadCameraParameters(const std::vector<std::string>& _param_name);

			/**
			 * @brief mapからプレイヤーパラメータを取得する
			 *
			 * @param  _param_name　パラメータ名（jsonに記載）
			 * @return   パラメータ(float)
			 */
			float GetCameraParam(const std::string _param_name);

		private:
			//プレイヤーパラメータ記録用変数
			std::unordered_map<std::string, float> map_camera_param_;
		};
	}
}
