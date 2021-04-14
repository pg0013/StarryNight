/**
 * @file    PlayerShootState.h
 * @brief  プレイヤーの射撃状態クラス
 * 
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#pragma once
#include "PlayerState.h"

namespace starrynight
{
	namespace player
	{
		class Player;

		/**
		 * @brief プレイヤーの射撃状態クラス
		 */
		class PlayerShootState :
			public PlayerState
		{
		public:
			PlayerShootState();
			~PlayerShootState();

			/**
			 * @brief　 状態遷移時に一度行われる入り口処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void Enter(Player& _player) override;

			/**
			 * @brief　 状態遷移時に一度行われる終了処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void Exit(Player& _player) override;

			/**
			 * @brief　 状態遷移を行うか判定する入力処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void Input(Player& _player) override;

			/**
			* @brief　 毎フレーム行われる更新処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void Update(Player& _player) override;
		};
	}
}
