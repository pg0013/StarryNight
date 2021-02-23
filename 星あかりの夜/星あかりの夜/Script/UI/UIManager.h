/**
 * @file    UIManager.h
 * @brief  ゲーム内UI管理クラス
 *
 * @author Takuya Fujisawa
 * @date    202102/08
 */

#pragma once
#include"appframe.h"
#include"Shoot_UI.h"

namespace starrynight
{
	namespace ui
	{
		class UIManager
		{
		public:
			UIManager();
			~UIManager();

			/**
			 * @brief   初期化処理
			 *
			 * @return 初期化成功の可否
			 */
			void Initialize();

			/**
			 * @brief   終了処理
			 *
			 * @return 終了処理成功の可否
			 */
			void Terminate();

			/**
			 * @brief   演算処理
			 *
			 * @return 演算処理成功の可否
			 */
			void Process();

			/**
			 * @brief   描画処理
			 *
			 * @return 描画処理成功の可否
			 */
			void Render();

		public:
			//射撃ガイド表示クラス
			Shoot_UI shoot_ui_;

		private:
			//UIパラメータ管理クラス
			parameters::Parameters param_;
		};
	}
}
