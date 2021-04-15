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
		/**
		 * @brief  タイトルモードクラス
		 */
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
			 * @brief コントローラ入力処理
			 */
			void Input();

			/**
			 * @brief 次のモードに遷移する処理
			 */
			void NextMode();

			handle background_graph_;//背景画像
			handle start_graph_;//スタートボタン画像
			handle exit_graph_;//おわる画像
			handle startline_graph_;//ボタン下のライン画像
			handle charactor_graph_;//キャラクター画像

			::parameters::Parameters param_;//パラメータ読み込みクラス

			int cursol_;//メニュー選択カーソル
			int nextmode_;//次に遷移するモード
			int menu_num_;//メニュー項目数
			int nextmode_count_;//次のモードへ移行するカウンタ
			bool pushed_flag_;//メニュー選択済みフラグ
		};
	}
}