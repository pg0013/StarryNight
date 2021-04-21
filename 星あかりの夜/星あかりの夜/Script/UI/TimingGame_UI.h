/**
 * @file    TimingGame_UI.h
 * @brief  タイミングゲームのUIを表示するクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/01
 */

#pragma once
#include"appframe.h"

namespace
{
	constexpr double firststar_scale_speed_ = 0.01 * 2.5;
	constexpr double secondstar_scale_speed_ = 0.01 * 1.5;
	constexpr double thirdstar_scale_speed_ = 0.01 * 1.0;
}

namespace starrynight
{
	namespace ui
	{
		/**
		 * @brief  タイミングゲームのUIを表示するクラス
		 */
		class TimingGame_UI
		{
		public:
			TimingGame_UI();
			~TimingGame_UI();

			/**
			 * @brief   初期化処理
			 *
			 * @return 初期化成功の可否
			 */
			void Initialize();

			/**
			 * @brief   終了処理
			 *
			 * @return 終了処理成功の可否
			 */
			void Terminate();

			/**
			 * @brief   演算処理
			 *
			 * @return 演算処理成功の可否
			 */
			void Process();

			/**
			 * @brief   描画処理
			 *
			 * @return 描画処理成功の可否
			 */
			void Render();

			/**
			 * @brief　スコアを計算する
			 */
			void CalcurateScore();

			/**
			 * @brief タイミングゲームのUIを表示するフラグを設定する
			 *
			 * @param  _flag　trueでタイミングゲームUIを表示
			 */
			void SetDrawTimingGuide(const bool& _flag) { draw_timing_guide_ = _flag; }

			/**
			 * @brief プレイヤーが星を発射できるかどうかを判定するフラグを設定する
			 *
			 * @param  _flag　プレイヤーが星を発射できるかどうかを判定するフラグ
			 */
			void SetLaunchStarShoot(const bool& _flag) { launch_star_shoot_ = _flag; }

			/**
			 * @brief プレイヤーが星を発射できるかどうかを判定するフラグを取得する
			 *
			 * @return   プレイヤーが星を発射できるかどうかを判定するフラグ
			 *					trueで発射
			 */
			bool GetLaunchStarShoot() const { return launch_star_shoot_; }

			/**
			 * @brief プレイヤーが選択した星が何番星かを設定する
			 *
			 * @param  _star_frame_num	プレイヤーが選択した星のフレーム番号
			 */
			void SetSelectedStarNum(const int& _star_frame_num) { selected_star_num_ = _star_frame_num; }

			/**
			 * @brief プレイヤーが選択した星が何番星かを取得する
			 *
			 * @return   何番星か（1～３）
			 */
			int GetSelectedStarNum() const { return selected_star_num_; }

			/**
			 * @brief 星選択時にボタンを押したときのフレームを記録する
			 *
			 * @param  _frame	モードフレーム数
			 */
			void SetSelectStarFrame(const int& _frame) { select_star_frame_ = _frame; }

			/**
			 * @brief タイミングゲームのタイミング評価
			 */
			enum class TIMING_STATUS
			{
				EXCELLENT, GOOD, BAD
			};

		private:
			/**
			 * @brief タイミングゲームのタイミングを評価する
			 *
			 * @return　タイミングの評価
			 */
			TIMING_STATUS CheckTiming();

			handle circle_guide_;//タイミング判定画像
			handle timing_circle_;//タイミング判定の円
			handle timing_circle_white_;//タイミング判定の円
			handle timing_circle_red_;//残り一回の時のタイミング判定の円
			handle timing_judge_[3];//判定画像
			handle aim_middle_;//真ん中を狙え画像

			sound::Sound se_;//効果音

			bool draw_timing_guide_;//trueでタイミングゲージを描画するフラグ
			bool launch_star_shoot_;//trueでプレイヤーがスターを発射できるフラグ
			bool shrink_circle_flag_;//trueでタイミングゲージが縮小するフラグ

			int presssed_frame_;//タイミングゲームでBボタンが押された時のフレーム
			int selected_star_num_;//選択されたスター番号
			int player_star_num_;//プレイヤー所持スター数

			int select_star_frame_;//星が選択された時のフレーム数

			double timing_exrate_;//縮小ゲージの拡大率
		};
	}
}
