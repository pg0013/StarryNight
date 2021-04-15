/**
 * @file    PlayerMoveState.h
 * @brief  プレイヤーの移動状態クラス
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
		 * @brief プレイヤーの移動状態クラス
		 */
		class PlayerMoveState :
			public PlayerState
		{
		public:
			PlayerMoveState(Player& _player);
			~PlayerMoveState();

			/**
			 * @brief 状態遷移時に一度行われる入り口処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			virtual void Enter(Player& _player) override;

			/**
			 * @brief 状態遷移時に一度行われる終了処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			virtual void Exit(Player& _player) override;

			/**
			 * @brief 状態遷移を行うか判定する入力処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			virtual void Input(Player& _player) override;

			/**
			* @brief 毎フレーム行われる更新処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			virtual void Update(Player& _player) override;

		private:
			/**
			 * @brief プレイヤーがWalkかRunかを設定する
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void SwitchMoveStatus(Player& _player);

		protected:
			/**
			 * @brief 移動処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			virtual void Move(Player& _player);

			/**
			 * @brief 右スティック入力からプレイヤーの方向を決める
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void DecideForwardDirection(Player& _player);

			/**
			 * @brief 壁ずり処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			virtual void SlidingWall(Player& _player);

			/**
			 * @brief 壁ずり後の押し出し処理
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void WallPush(Player& _player);

			VECTOR move_;//移動量
			VECTOR position_;//プレイヤー座標
			VECTOR old_position_;//移動前の座標

			float walk_speed_;//移動速度
			float run_speed_;//移動速度
			float rot_speed_;//回転速度
		};
	}
}
