/**
 * @file    ModePauseMenu.h
 * @brief  ゲーム中のポーズ画面シーン
 *
 * @author Takuya Fujisawa
 * @date   2021/03/06
 */

#pragma once
#include "appframe.h"

namespace
{
	constexpr int TUTORIAL = 0;
	constexpr int RETURN = 1;
	constexpr int TITLE = 2;
}

namespace starrynight
{
	namespace mode
	{
		class ModePauseMenu :
			public ::mode::ModeBase
		{
		public:
			ModePauseMenu();
			~ModePauseMenu();

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

			handle background_graph_;//背景画像
			handle overlay_graph_;//背景画像
			handle pause_graph_;
			handle return_title_graph_[2];
			handle return_graph_[2];
			handle tutorial_graph_[2];

			::parameters::Parameters param_;//パラメータ読み込みクラス

			int cursol_;
			int nextmode_;
			int menu_num_;
			int nextmode_count_;
			bool pushed_flag_;
			int select_frame_;
		};

	}
}

