/**
 * @file    ModeTitle.h
 * @brief  タイトルモードクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/03
 */

#pragma once
#include "appframe.h"

namespace
{
	constexpr int MENU = 0;
	constexpr int EXIT = 1;
}

namespace starrynight
{
	namespace mode
	{
		class ModeTitle :
			public ::mode::ModeBase
		{
		public:
			ModeTitle();
			~ModeTitle();

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
			handle start_graph_;//スタートボタン画像
			handle startline_graph_;//ボタン下のライン画像
			handle charactor_graph_;//キャラクター画像

			::parameters::Parameters param_;//パラメータ読み込みクラス

			bool pushed_flag_;//ボタンが押されたときのフラグ
			int nextmode_count_;//次のモードに遷移するまでの時間
			int nextmode_;
		};
	}
}