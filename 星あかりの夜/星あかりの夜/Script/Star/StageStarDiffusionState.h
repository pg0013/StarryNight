/**
 * @file    StageStarDiffusionState.h
 * @brief  フィールド内スターの拡散状態クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#pragma once
#include "StageStarState.h"

namespace starrynight
{
	namespace star
	{
		class Star;

		/**
		 * @brief  フィールド内スターの拡散状態クラス
		 */
		class StageStarDiffusionState :
			public StageStarState
		{
		public:
			StageStarDiffusionState();
			~StageStarDiffusionState();

			/**
			 * @brief　 状態遷移時に一度行われる入り口処理
			 *
			 * @param  _star　インスタンスを持つクラスの参照
			 */
			void Enter(Star& _star) override;

			/**
			 * @brief　 状態遷移時に行われる出口処理
			 *
			 * @param  _star　インスタンスを持つクラスの参照
			 */
			void Exit(Star& _star) override;

			/**
			 * @brief　 状態遷移を行うか判定する入力処理
			 *
			 * @param  _star　インスタンスを持つクラスの参照
			 */
			void Input(Star& _star) override;

			/**
			 * @brief　 毎フレーム行われる更新処理
			 *
			 * @param  _star インスタンスを持つクラスの参照
			 */
			void Update(Star& _star) override;

		private:
			/**
			 * @brief　プレイヤーの元から散らばる処理.
			 */
			void Diffusion(Star& _star);
		};
	}
}