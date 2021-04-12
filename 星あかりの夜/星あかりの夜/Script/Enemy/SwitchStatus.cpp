/**
 * @file    SwitchStatus.cpp
 * @brief  プレイヤーの状態遷移処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include "Enemy.h"
#include"../Player/Player.h"
#include"../Effect/DamageEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::enemy;

void Enemy::SwitchStatus()
{
	EnemyState* state = enemy_state_->Input(*this);

	if (state != nullptr)
	{
		//現在の状態の終了処理を実行
		enemy_state_->Exit(*this);

		//現在の状態から次の状態へ遷移
		delete enemy_state_;
		enemy_state_ = state;

		//移行した状態の初期化処理を実行
		enemy_state_->Enter(*this);
	}
}