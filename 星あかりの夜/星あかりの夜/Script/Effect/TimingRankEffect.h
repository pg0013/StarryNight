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
			virtual void Initialize();

			/**
			 * @brief　演算処理.
			 */
			virtual void Process();

			/**
			 * @brief　描画処理.
			 */
			virtual void Render();

		private:
			int start_frame_;//エフェクト再生開始フレーム
		};
	}
}