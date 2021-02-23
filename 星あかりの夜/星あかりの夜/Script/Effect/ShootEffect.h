/**
 * @file    ShootEffect.h
 * @brief  射撃の流星エフェクト
 *
 * @author Takuya Fujisawa
 * @date   2021/02/11
 */

#include"appframe.h"
#include "EffectBase.h"

namespace starrynight
{
	namespace effect
	{
		class ShootEffect :
			public effect::EffectBase
		{
		public:
			ShootEffect();
			~ShootEffect();

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
		};
	}
}