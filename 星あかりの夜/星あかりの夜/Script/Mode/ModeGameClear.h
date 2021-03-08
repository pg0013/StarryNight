/**
 * @file    ModeGameClear.h
 * @brief  ゲームクリアシーン
 *
 * @author Takuya Fujisawa
 * @date   2021/03/05
 */

#pragma once
#include "appframe.h"
#include"../UI/Score_UI.h"

namespace
{
	constexpr int RETURN_TITLE = 0;
	constexpr int NEXT_GAME = 1;
}

namespace starrynight
{
	namespace mode
	{
		class ModeGameClear :
			public ::mode::ModeBase
		{
		public:
			ModeGameClear(int _score);
			~ModeGameClear();

			/**
			 * @brief   初期化処理
			 * モードサーバー登録時に一度だけ実行される
			 *
			 * @return 初期化成功の可否
			 */
			virtual bool Initialize();

			/**
			 * @brief   終了処理
			 * モードサーバーから削除時に一度だけ実行される
			 *
			 * @return 終了処理成功の可否
			 */
			virtual bool Terminate();

			/**
			 * @brief   演算処理
			 *
			 * @return 演算処理成功の可否
			 */
			virtual bool Process();

			/**
			 * @brief   描画処理
			 *
			 * @return 描画処理成功の可否
			 */
			virtual bool Render();

		private:
			/**
			 * @brief　 コントローラ入力処理
			 */
			void Input();

			/**
			 * @brief　 次のモードに遷移する処理
			 */
			void NextMode();

		private:
			handle background_graph_;//背景画像
			handle gameclear_graph_;//キャラクター画像
			handle next_graph_;//キャラクター画像
			handle next_base_graph_;//キャラクター画像
			handle return_graph_;//キャラクター画像
			handle return_base_graph_;

			::parameters::Parameters param_;//パラメータ読み込みクラス

			ui::Score_UI score_ui_;

			int cursol_;
			int menu_num_;
			int nextmode_count_;
			bool pushed_flag_;
		};
	}
}