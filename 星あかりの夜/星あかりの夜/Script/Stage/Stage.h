/**
 * @file    Stage.h
 * @brief  ステージ管理クラス
 *
 * @author Takuya Fujisawa
 * @date    202012/16
 */

#pragma once
#include"appframe.h"
#include"StageParameters.h"
#include<vector>

namespace starrynight
{
	namespace stage
	{
		class Stage
		{
		public:
			Stage();
			~Stage();

			/**
			 * @brief　初期化処理.
			 */
			void Initialize();

			/**
			 * @brief　演算処理.
			 */
			void Process();

			/**
			 * @brief　描画処理.
			 *
			 */
			void Render();

			/**
			 * @brief　ステージのモデルハンドルを削除.
			 *
			 */
			void ClearHandle();

			handle GetFieldHandle() { return field_; }

		private:
			StageParameters stage_param_;
			std::vector<handle> stage_handle_;
			handle navimesh_;
			handle field_;
		};
	}
}