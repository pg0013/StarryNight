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
#include"TimingGame_UI.h"
#include"Score_UI.h"
#include"Hp_UI.h"

namespace starrynight
{
	namespace ui
	{
		/**
		 * @brief  ゲーム内UI管理クラス
		 */
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

			/**
			 * @brief　 UIの描画をするか設定するフラグ
			 *
			 * @param  _flag	UI描画フラグ　trueで描画
			 */
			void SetDrawUIFlag(const bool& _flag) { draw_ui_flag_ = _flag; }

			/**
			 * @brief　 UIの描画をするか設定するフラグ
			 *
			 * @param  _flag	UI描画フラグ　trueで描画
			 */
			void SetDrawPlayerUIFlag(const bool& _flag) { draw_player_ui_flag_ = _flag; }

		public:
			//射撃ガイド表示クラス
			Shoot_UI shoot_ui_;

			//タイミングゲーム表示クラス
			TimingGame_UI timing_ui_;

			//ゲームスコア表示クラス
			Score_UI score_ui_;

			//プレイヤーHP表示クラス
			Hp_UI hp_ui_;

		private:
			//UIパラメータ管理クラス
			parameters::Parameters param_;

			bool draw_ui_flag_;//UIを描画するか設定するフラグ
			bool draw_player_ui_flag_;//プレイヤーUIを描画するか設定するフラグ
		};
	}
}
