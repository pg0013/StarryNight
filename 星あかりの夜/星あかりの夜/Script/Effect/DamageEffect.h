/**
 * @file    DamageEffect.h
 * @brief  プレイヤーのダメージエフェクトクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#pragma once
#include"appframe.h"
#include "EffectBase.h"

namespace starrynight
{
	namespace effect
	{
		/**
		 * @brief  プレイヤーのダメージエフェクトクラス
		 */
		class DamageEffect :
			public EffectBase
		{
		public:
			DamageEffect();
			~DamageEffect();

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
			int start_frame_;//生成時のフレーム
		};
	}
}