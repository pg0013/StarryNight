/**
 * @file    ShootEffect.h
 * @brief  射撃の流星エフェクト
 *
 * @author Takuya Fujisawa
 * @date   2021/02/11
 */

#pragma once
#include"appframe.h"
#include "EffectBase.h"

namespace starrynight
{
	namespace effect
	{
		/**
		 * @brief  射撃の流星エフェクト
		 */
		class ShootEffect :
			public effect::EffectBase
		{
		public:
			ShootEffect(VECTOR _target);
			~ShootEffect();

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
			/**
			 * @brief　 射撃対象へ向く回転値を設定する
			 *
			 */
			void SetRotationToTarget();

			VECTOR shoot_target_;//射撃対象の座標
			sound::PlayWAVE se_;//効果音
		};
	}
}