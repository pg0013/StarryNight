/**
 * @file    EnemyEscapeState.cpp
 * @brief  敵のプレイヤーから離れていく状態クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/12
 */

#include "EnemyEscapeState.h"
#include"EnemyWaitState.h"
#include"../Player/Player.h"
#include"../Effect/DamageEffect.h"
#include"../Mode/ModeGame.h"
#include"Enemy.h"
using namespace starrynight::enemy;

EnemyEscapeState::EnemyEscapeState()
{
}

EnemyEscapeState::~EnemyEscapeState()
{
}

void EnemyEscapeState::Enter(Enemy& _enemy)
{
	VECTOR rotation = _enemy.GetRotation();
	rotation.y -= DEG2RAD(180.0f);
	_enemy.SetRotation(rotation);
}

void EnemyEscapeState::Exit(Enemy& _enemy)
{
}

EnemyState* EnemyEscapeState::Input(Enemy& _enemy)
{
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	float player_distance = VSize(VSub(player_position, _enemy.GetPosition()));

	//プレイヤーから一定距離離れたら待機状態へ移行
	if (player_distance > _enemy.GetDetectLength() + 300.0f)
	{
		return  NEW EnemyWaitState();
	}

	//現在の状態を継続
	_enemy.SetAnimStatus(Enemy::ANIM_STATUS::MOVE);
	_enemy.SetMoveStatus(Enemy::MOVE_STATUS::ESCAPE);
	return nullptr;
}

void EnemyEscapeState::Update(Enemy& _enemy)
{
	VECTOR rotation = _enemy.GetRotation();
	VECTOR move = { 0,0,0 };

	move = VScale(VNorm(utility::GetForwardVector(rotation.y)), _enemy.GetRunSpeed());
	_enemy.Move(move);
}