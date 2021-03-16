/**
 * @file    StageFlowEffect.h
 * @brief  ステージ上に漂う花びらなどのエフェクト
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
		 * @brief  ステージ上に漂う花びらなどのエフェクト
		 */
		class StageFlowEffect :
			public EffectBase
		{
		public:
			StageFlowEffect();
			~StageFlowEffect();

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
		};
	}
}



