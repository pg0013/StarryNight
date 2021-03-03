/**
 * @file    ShootPointEffect.h
 * @brief  射撃ポイントエフェクトクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/19
 */

#pragma once
#include"appframe.h"
#include "EffectBase.h"

namespace starrynight
{
	namespace effect
	{
		class ShootPointEffect :
			public effect::EffectBase
		{
		public:
			ShootPointEffect();
			~ShootPointEffect();

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
			//1frameのみ処理するためのフラグ
			bool once_flag_;

			float scale_y_;//y軸方向のスケール値
			float min_scale_y_;//y軸方向のスケール最大値
			float max_scale_y_;//y軸方向のスケール最小値
		};
	}
}
