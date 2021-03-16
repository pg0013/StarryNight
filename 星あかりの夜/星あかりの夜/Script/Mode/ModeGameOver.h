/**
 * @file    ModeGameOver.h
 * @brief  ゲームオーバーモードクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/08
 */

#pragma once
#include "appframe.h"

namespace
{
	constexpr int GIVEUP = 0;
	constexpr int AGAIN = 1;
}

namespace starrynight
{
	namespace mode
	{
		/**
		 * @brief  ゲームオーバーモードクラス
		 */
		class ModeGameOver :
			public ::mode::ModeBase
		{
		public:
			ModeGameOver(std::string _stage_name);
			~ModeGameOver();

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
			handle again_graph_;//キャラクター画像
			handle return_graph_;//キャラクター画像

			::parameters::Parameters param_;//パラメータ読み込みクラス

			int cursol_;//メニュー選択カーソル
			int menu_num_;//メニュー項目数
			int nextmode_count_;//次のモードへ移行するカウンタ
			bool pushed_flag_;//メニュー選択済みフラグ

			std::string stage_name_;//ステージ名
		};
	}
}
