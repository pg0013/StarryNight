/**
 * @file    StageStarFollowState.h
 * @brief  フィールド内スターのプレイヤー追跡クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#pragma once
#include "StageStarState.h"
#include<queue>

namespace starrynight
{
	namespace star
	{
		class Star;

		/**
		 * @brief  フィールド内スターのプレイヤー追跡クラス
		 */
		class StageStarFollowState :
			public StageStarState
		{
		public:
			StageStarFollowState();
			~StageStarFollowState();

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
			 * @brief　プレイヤーの後ろをついてくる処理.
			 */
			void Follow(Star& _star);

			float follow_interval_;//プレイヤーやひとつ前の星との間隔

			std::queue<VECTOR> player_pos_history_;//プレイヤーの位置ベクトル記録用コンテナ
			VECTOR old_player_position_;//前フレームのプレイヤーの位置ベクトルを記録する
		};
	}
}