/**
 * @file    ModePauseMenu.h
 * @brief  ゲーム中のポーズ画面モードクラス
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
		/**
		 * @brief  ゲーム中のポーズ画面モードクラス
		 */
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
			bool Initialize() override;

			/**
			 * @brief   終了処理
			 * モードサーバーから削除時に一度だけ実行される
			 *
			 * @return 終了処理成功の可否
			 */
			bool Terminate() override;

			/**
			 * @brief   演算処理
			 *
			 * @return 演算処理成功の可否
			 */
			bool Process() override;

			/**
			 * @brief   描画処理
			 *
			 * @return 描画処理成功の可否
			 */
			bool Render() override;

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
			handle overlay_graph_;//オーバーレイ用画像
			handle pause_graph_;//ポーズ画像
			handle return_title_graph_[2];//タイトルへ戻る
			handle return_graph_[2];//ゲームへ戻る
			handle tutorial_graph_[2];//チュートリアルへ移行

			::parameters::Parameters param_;//パラメータ読み込みクラス

			int cursol_;//メニュー選択カーソル
			int nextmode_;//次に遷移するモード
			int menu_num_;//メニュー項目数
			int nextmode_count_;//次のモードへ移行するカウンタ
			bool pushed_flag_;//メニュー選択済みフラグ
			int select_frame_;//カーソルを動かしたときのフレーム
		};

	}
}

