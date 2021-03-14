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

			/**
			 * @brief　 スコア画面の下地を描画するか設定する
			 *
			 * @param  _flag　trueで下地を描画する
			 */
			void SetDrawScoreBaseFlag(bool _flag) { draw_scorebase_flag_ = _flag; }

			/**
			 * @brief　 スコアを点滅表示するフラグを設定する
			 *
			 * @param  _flag　点滅表示フラグ
			 */
			void SetBlinkingFlag(bool _flag)
			{
				blinking_flag_ = _flag;
				blink_startframe_ = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount();
			}

			/**
			 * @brief　UIの位置を設定する.
			 *
			 * @param _positon 位置(x,y,z)のベクトル構造体,zは無効
			 */
			void SetPosition(VECTOR _positon) { position_ = _positon; }

			/**
			 * @brief　 拡大率を設定する
			 *
			 * @param  _rate	拡大率
			 */
			void SetEXRate(double _rate) { exrate_ = _rate; }

			/**
			 * @brief　 プレイヤースコアを設定する
			 *
			 * @param  _score　ゲームスコア
			 */
			void SetPlayerScore(int _score) { player_score_ = _score; }

			/**
			 * @brief　 プレイヤースコアを更新する
			 */
			void UpdatePlayerScore();

		private:
			/**
			 * @brief　 スコアの桁の値を設定する
			 */
			void SetScoreRankNum();

			VECTOR position_;//座標
			double exrate_;//拡大率

			handle score_num_graph_[10] = { 0 };//数字画像
			handle score_base_graph_;//下地画像
			handle star_count_graph_;//残り何個画像

			sound::PlayWAVE se_;//効果音

			int player_score_;//プレイヤースコア
			int rank_value_[6] = { 0 };//スコアの桁の値 0が一の位
			bool draw_rank_flag_[6] = { 0 };//桁を描画するか決めるフラグ　0が一の位
			bool draw_scorebase_flag_;//スコアの下地画像を描画するフラグ
			bool blinking_flag_;//点滅表示フラグ
			int blink_startframe_;//点滅開始フレーム
		};
	}
}
