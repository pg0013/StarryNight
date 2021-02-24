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
		class DamageEffect :
			public EffectBase
		{
		public:
			DamageEffect();
			~DamageEffect();

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
			int start_frame_;//生成時のフレーム
		};
	}
}