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
		/**
		 * @brief  星座エフェクトクラス
		 */
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
			void Initialize() override;

			/**
			 * @brief　演算処理.
			 */
			void Process() override;

			/**
			 * @brief　描画処理.
			 */
			void Render() override;

			/**
			 * @brief 星座完成エフェクトを描画する
			 */
			void DrawCompleteEffect();
		private:
			/**
			 * @brief　SEを再生する.
			 */
			void PlayEffectSound();

			/**
			 * @brief エフェクトをプレイヤーの方向に向ける
			 */
			void SetEffectRotationToPlayer();

			sound::Sound se1_;//効果音
			sound::Sound se2_;//効果音

			std::string skystar_name_;//生成する星座名
			handle after_effect_resource_[3];//完成星座エフェクトのリソースハンドル

			bool once_flag_;//一度処理するためのフラグ
			int start_frame_;//星座カメラモードに変更された時のフレーム
			int select_star_num_;//プレイヤーが選択した星番号（何番星か）

			int switch_effect_frame_;//完成星座エフェクトに切り替えるフレーム
			int complete_effect_startframe_;//完成エフェクトを生成するフレーム
		};
	}
}