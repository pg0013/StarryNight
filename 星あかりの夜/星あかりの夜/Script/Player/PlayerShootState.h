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
			 * @brief	パチンコを構えて星を撃つ処理.
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void HoldSlingShot(Player& _player);

			/**
			 * @brief パチンコを構える処理.
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void SlingShotStance(Player& _player);

			/**
			 * @brief 星を発射する処理.
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 * @param _star_position 射撃する星の座標
			 */
			void Launch_Star(Player& _player, const VECTOR& _star_position);

			/**
			 * @brief 射撃構え時のプレイヤーの向きを設定する.
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 */
			void SetShootRotation(Player& _player);

			/**
			 * @brief 星座との当たり判定を取得する
			 *
			 * @param  _player インスタンスを持つクラスの参照
			 * @return 星座との当たり判定構造体
			 */
			MV1_COLL_RESULT_POLY CheckHitStar(Player& _player);

			bool shoot_charge_effect_flag_;//射撃溜めエフェクト生成フラグ
		};
	}
}
