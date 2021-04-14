/**
 * @file    StageStarState.h
 * @brief  ステージスターの状態基底クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#pragma once
#include"appframe.h"

namespace starrynight
{
	namespace star
	{
		class Star;

		/**
		 * @brief  ステージスターの状態基底クラス
		 */
		class StageStarState :
			public state::StateBase
		{
		public:
			StageStarState() {}
			virtual ~StageStarState() {}

			/**
			 * @brief　 状態遷移時に一度行われる入り口処理
			 *
			 * @param  _star　インスタンスを持つクラスの参照
			 */
			virtual void Enter(Star& _star) {}

			/**
			 * @brief　 状態遷移時に行われる出口処理
			 *
			 * @param  _star　インスタンスを持つクラスの参照
			 */
			virtual void Exit(Star& _star) {}

			/**
			 * @brief　 状態遷移を行うか判定する入力処理
			 *
			 * @param  _star　インスタンスを持つクラスの参照
			 */
			virtual void Input(Star& _star) {}

			/**
			 * @brief　 毎フレーム行われる更新処理
			 *
			 * @param  _star インスタンスを持つクラスの参照
			 */
			virtual void Update(Star& _star) {}
		};
	}
}