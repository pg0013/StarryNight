/**
 * @file    SwitchStatus.cpp
 * @brief  ƒvƒŒƒCƒ„[‚Ìó‘Ô‘JˆÚˆ—
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
	enemy_state_->Input(*this);
}