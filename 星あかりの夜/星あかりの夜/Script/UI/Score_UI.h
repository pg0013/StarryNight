/**
 * @file    Score_UI.h
 * @brief  ゲームスコア表示UIクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/03
 */

#pragma once
#include"appframe.h"

namespace
{
	constexpr int score_base_x = 0;
	constexpr int score_base_y = 0;
	constexpr int score_num_x = 1800;
	constexpr int score_num_y = 100;
}

namespace starrynight
{
	namespace ui
	{
		class Score_UI
		{
		public:
			Score_UI();
			~Score_UI();

			/**
			 * @brief   初期化処理
			 */
			void Initialize();

			/**
			 * @brief   終了処理
			 */
			void Terminate();

			/**
			 * @brief   演算処理
			 */
			void Process();

			/**
			 * @brief   描画処理
			 */
			void Render();

		private:
			/**
			 * @brief　 プレイヤースコアを更新する
			 */
			void UpdatePlayerScore();

			/**
			 * @brief　 スコアの桁の値を設定する
			 */
			void SetScoreRankNum();

			int player_score_;//プレイヤースコア
			int rank_value_[6] = { 0 };//スコアの桁の値 0が一の位
			bool draw_rank_flag_[6] = { 0 };//桁を描画するか決めるフラグ　0が一の位

			handle score_base_graph_;
			handle score_num_graph_[10] = { 0 };
		};
	}
}
