/**
 * @file    PlayerTrailEffect.h
 * @brief  プレイヤーの軌跡エフェクト
 *
 * @author Takuya Fujisawa
 * @date   2021/03/13
 */

#pragma once
#include"appframe.h"
#include "EffectBase.h"

namespace starrynight
{
	namespace effect
	{
		/**
		 * @brief  プレイヤーの軌跡エフェクト
		 */
		class PlayerTrailEffect :
			public EffectBase
		{
		public:
			PlayerTrailEffect();
			~PlayerTrailEffect();

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
			bool pause_flag_;//エフェクト一時停止フラグ
		};
	}
}
