/**
 * @file    PlayerState.h
 * @brief  プレイヤーの状態遷移の基底クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#pragma once
#include"appframe.h"

namespace starrynight
{
	namespace player
	{
		class Player;

		/**
		 * @brief プレイヤーの状態基底クラス
		 */
		class PlayerState :
			public state::StateBase
		{
		public:
			virtual ~PlayerState(){}

			/**
			 * @brief　 状態遷移時に一度行われる入り口処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			virtual void Enter(Player& _player) {}

			/**
			 * @brief　 状態遷移時に一度行われる終了処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			virtual void Exit(Player& _player) {}

			/**
			 * @brief　 状態遷移を行うか判定する入力処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			virtual void Input(Player& _player) { }

			/**
			* @brief　 毎フレーム行われる更新処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			virtual void Update(Player& _player) {}

		};
	}
}
