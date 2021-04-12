/**
 * @file    ModeGameClear.h
 * @brief  ゲームクリアモードクラス
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
		/**
		 * @brief  ゲームクリアモードクラス
		 */
		class ModeGameClear :
			public ::mode::ModeBase
		{
		public:
			ModeGameClear(const int& _score, const std::string& _stage_name);
			~ModeGameClear();

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

		private:
			handle background_graph_;//背景画像
			handle gameclear_graph_;//ゲームクリア画像
			handle next_graph_;//つぎへ
			handle next_base_graph_;//つぎへ下地
			handle return_graph_;//タイトルへ戻る
			handle return_base_graph_;//タイトルへ戻る下地

			::parameters::Parameters param_;//パラメータ読み込みクラス

			ui::Score_UI score_ui_;//スコア表示クラス

			std::string stage_name_;//ステージ名

			sound::PlayWAVE se_;//効果音

			int cursol_;//メニュー選択カーソル
			int menu_num_;//メニュー項目数
			int nextmode_count_;//次のモードへ移行するカウンタ
			bool pushed_flag_;//メニュー選択済みフラグ
		};
	}
}