/**
 * @file    ModeMenu.h
 * @brief  メニュー選択モードクラス
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

	constexpr int icon_x = 550;
	constexpr int icon_y = 200;
	constexpr int stage_icon_x = 1350;
	constexpr int stage_icon_y = 350;
	constexpr int stage_goal_x = 550;
	constexpr int stage_goal_y = 500;
}

namespace starrynight
{
	namespace mode
	{
		/**
		 * @brief  メニュー選択モードクラス
		 */
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
			handle stage_num_graph_[3];//ステージ数
			handle stage_graph_[3];//ステージサムネイル
			handle stage_text_[3];//ステージ補足テキスト画像
			handle spring_icon_;//春アイコン
			handle cloud_graph_;//下地の雲
			handle menu_graph_;//下地の本
			handle stage_goal_graph_;//ステージサムネイル

			::parameters::Parameters param_;//パラメータ読み込みクラス

			int cursol_;//メニュー選択カーソル
			int nextmode_;//次に遷移するモード
			int menu_num_;//メニュー項目数
			int nextmode_count_;//次のモードへ移行するカウンタ
			bool pushed_flag_;//メニュー選択済みフラグ
		};
	}
}
