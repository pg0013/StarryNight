/**
 * @file    ShootChargeEffect.h
 * @brief  射撃構え時のエフェクトクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/09
 */

#pragma once
#include"appframe.h"
#include "EffectBase.h"

namespace starrynight
{
	namespace effect
	{
		/**
		 * @brief  射撃構え時のエフェクトクラス
		 */
		class ShootChargeEffect :
			public effect::EffectBase
		{
		public:
			ShootChargeEffect();
			~ShootChargeEffect();

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
			sound::PlayWAVE se_;//効果音
		};
	}
}
