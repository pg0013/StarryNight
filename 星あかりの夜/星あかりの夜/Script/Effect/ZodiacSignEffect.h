/**
 * @file    ZodiacSignEffect.h
 * @brief  星座エフェクトクラス
 *
 * @author Takuya Fujisawa
 * @date    202103/02
 */

#pragma once
#include"appframe.h"
#include "EffectBase.h"
#include<string>

namespace starrynight
{
	namespace effect
	{
		class ZodiacSignEffect :
			public EffectBase
		{
		public:
			/**
			 * @brief 星座エフェクトを生成する.
			 *
			 * @param _sign_name	星座名
			 */
			ZodiacSignEffect(std::string _sign_name);
			~ZodiacSignEffect();

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

			/**
			 * @brief　 星座完成エフェクトを描画する
			 */
			void DrawCompleteEffect();
		private:
			std::string skystar_name;
			handle after_effect_resource_[3];

			bool once_flag_;
			int start_frame_;
			int select_star_num_;
		};
	}
}