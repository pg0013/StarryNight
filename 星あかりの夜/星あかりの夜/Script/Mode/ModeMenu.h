/**
 * @file    ModeMenu.h
 * @brief  メニュー選択シーン
 *
 * @author Takuya Fujisawa
 * @date   2021/03/06
 */

#pragma once
#include "appframe.h"

namespace
{
	constexpr int HARU_A = 0;
	constexpr int HARU_B = 1;
	constexpr int HARU_C = 2;
	constexpr int TITLE = 3;

	constexpr int icon_x = 500;
	constexpr int icon_y = 150;
	constexpr int stage_icon_x = 1350;
	constexpr int stage_icon_y = 350;

}

namespace starrynight
{
	namespace mode
	{
		class ModeMenu :
			public ::mode::ModeBase
		{
		public:
			ModeMenu();
			~ModeMenu();

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
			handle stage_num_graph_[3];
			handle stage_graph_[3];
			handle spring_icon_;
			handle cloud_graph_;
			handle menu_graph_;

			::parameters::Parameters param_;//パラメータ読み込みクラス

			int cursol_;
			int nextmode_;
			int menu_num_;
			int nextmode_count_;
			bool pushed_flag_;

		};

	}
}

