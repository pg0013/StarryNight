/**
 * @file    PlayerJumpState.h
 * @brief  プレイヤーのジャンプ状態クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#pragma once
#include "PlayerMoveState.h"

namespace starrynight
{
	namespace player
	{
		class Player;

		/**
		 * @brief プレイヤーのジャンプ状態クラス
		 */
		class PlayerJumpState :
			public PlayerMoveState
		{
		public:
			PlayerJumpState(Player& _player);
			~PlayerJumpState();

			/**
			 * @brief 状態遷移時に一度行われる入り口処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void Enter(Player& _player) override;

			/**
			 * @brief 状態遷移時に一度行われる終了処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void Exit(Player& _player) override;

			/**
			 * @brief 状態遷移を行うか判定する入力処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void Input(Player& _player) override;

			/**
			* @brief 毎フレーム行われる更新処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void Update(Player& _player) override;

		private:
			/**
			 * @brief ジャンプ処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void Jump(Player& _player);

			/**
			 * @brief ジャンプ中のプレイヤー移動処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void Move(Player& _player) override;

			/**
			 * @brief プレイヤー着地処理
			 *
			 * @param _player
			 */
			void Landing(Player& _player);

			/**
			 * @brief 壁ずり処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void SlidingWall(Player& _player) override;

			/**
			 * @brief プレイヤーのジャンプ状態を設定
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void SwitchJumpState(Player& _player);

			float jump_speed_;//ジャンプ速度
			float jump_height_;//ジャンプ高さ
			float gravity_;//重力
		};
	}
}
