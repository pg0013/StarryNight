/**
 * @file    TimingRankEffect.h
 * @brief  タイミング判定によって変わる射撃時のエフェクト
 *
 * @author Takuya Fujisawa
 * @date   2021/03/05
 */

#pragma once
#include"appframe.h"
#include "EffectBase.h"
#include"../Mode/ModeGame.h"
#include"../UI/TimingGame_UI.h"

namespace starrynight
{
	namespace effect
	{
		/**
		 * @brief  タイミング判定によって変わる射撃時のエフェクト
		 */
		class TimingRankEffect :
			public EffectBase
		{
		public:
			TimingRankEffect(mode::ModeGame::SCORE_RANK _rank);
			~TimingRankEffect();

			/**
			 * @brief　初期化処理.
			 */
			void Initialize() override;

			/**
			 * @brief　演算処理.
			 */
			void Process() override;

			/**
			 * @brief　描画処理.
			 */
			void Render() override;

		private:
			int start_frame_;//エフェクト再生開始フレーム
		};
	}
}