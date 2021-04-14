/**
 * @file    StageStarWaitState.h
 * @brief  フィールド内スターの待機状態クラス
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
		 * @brief フィールド内スターの待機状態クラス
		 */
		class StageStarWaitState :
			public StageStarState
		{
		public:
			StageStarWaitState();
			~StageStarWaitState();

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
			 * @brief 待機処理.
			 */
			void Wait(Star& _star);

			float touch_length_;//ステージスターの取得範囲
			float detect_length_;//ステージスターの検出範囲
		};
	}
}