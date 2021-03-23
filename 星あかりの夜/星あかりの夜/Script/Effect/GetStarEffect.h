/**
 * @file    GetStarEffect.h
 * @brief  スターを獲得したときのエフェクトクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/03
 */

#pragma once
#include"appframe.h"
#include "EffectBase.h"

namespace starrynight
{
	namespace effect
	{
		/**
		 * @brief  スターを獲得したときのエフェクトクラス
		 */
		class GetStarEffect :
			public EffectBase
		{
		public:
			GetStarEffect();
			~GetStarEffect();

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