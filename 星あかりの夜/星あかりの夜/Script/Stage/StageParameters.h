/**
 * @file    StageParameters.h
 * @brief  ステージパラメータ読み込みクラス
 *
 * @author Takuya Fujisawa
 * @date   2020/12/17
 */
#pragma once
#include"appframe.h"

namespace starrynight
{
	namespace stage
	{
		/**
		 * @brief  ステージパラメータ読み込みクラス
		 */
		class StageParameters :
			public parameters::Parameters
		{
		public:
			StageParameters();
			~StageParameters();

			/**
			 * @brief　 ステージパラメータの読み込み
			 *
			 * @param  _stagename　ステージ名
			 * @param  _async_flag　非同期読み込みフラグ
			 */
			void LoadStage(const std::string& _stagename, const bool& _async_flag = false);
		};
	}
}