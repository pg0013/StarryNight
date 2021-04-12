/**
 * @file    Shoot_UI.h
 * @brief  射撃時のUI描画処理を行うクラス
 *
 * @author Takuya Fujisawa
 * @date    202102/08
 */

#pragma once
#include"appframe.h"

namespace starrynight
{
	namespace ui
	{
		/**
		 * @brief  射撃時のUI描画処理を行うクラス
		 */
		class Shoot_UI
		{
		public:
			Shoot_UI();
			~Shoot_UI();

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

			/**
			 * @brief　 射撃用ガイドを表示するフラグを設定する
			 *
			 * @param  _flag	trueで射撃ガイド表示
			 */
			void SetDrawShootGuide(const bool& _flag) { draw_shoot_guide_ = _flag; }

			/**
			 * @brief　 星座を狙えを表示するフラグを設定する
			 *
			 * @param  _flag	trueで射撃ガイド表示
			 */
			void SetDrawChooseSkystarGuide(const bool& _flag) { draw_choose_skystar_guide_ = _flag; }

		private:
			//射撃用ガイドを表示するフラグ
			bool draw_shoot_guide_;
			//星座を狙えを表示するフラグ
			bool draw_choose_skystar_guide_;

			//画像用ハンドル
			handle shoot_guide_;
			handle choose_skystar_;
		};
	}
}
