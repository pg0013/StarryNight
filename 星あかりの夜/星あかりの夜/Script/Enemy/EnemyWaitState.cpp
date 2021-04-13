/**
 * @file    EnemyWaitState.cpp
 * @brief  敵の待機状態クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/12
 */

#include "EnemyWaitState.h"
#include"EnemyTrackingState.h"
#include"../Player/Player.h"
#include"Enemy.h"
#include"../Mode/ModeGame.h"
using namespace starrynight::enemy;

EnemyWaitState::EnemyWaitState()
{
	waitstart_frame_ = 0;
}

EnemyWaitState::~EnemyWaitState()
{
}

void EnemyWaitState::Enter(Enemy& _enemy)
{
	_enemy.SetMoveStatus(Enemy::MOVE_STATUS::WAIT);
	_enemy.SetAnimStatus(Enemy::ANIM_STATUS::WAIT);

	//待機状態開始フレームを初期化
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	waitstart_frame_ = mode_game->GetModeCount();
}

void EnemyWaitState::Exit(Enemy& _enemy)
{
}

void EnemyWaitState::Input(Enemy& _enemy)
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	//プレイヤーがダメージを受けたかどうか取得
	bool player_damaged = false;
	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(iter);
			player_damaged = player->GetDamageFlag();
			break;
		}
	}

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR enemy_position = _enemy.GetPosition();
	float player_distance = VSize(VSub(player_position, enemy_position));
	float player_distance_y = abs(player_position.y - enemy_position.y);

	//追跡距離範囲内であれば、追跡モードに移行
	//逃げるモードの時にはプレイヤーから離れるために追跡しない
	if (player_distance < _enemy.GetDetectLength())
	{
		if (player_distance_y < 200 &&
			player_damaged == false)
		{
			//追跡状態へ以降
			_enemy.ChangeEnemyState("Tracking");
		}
	}
}

void EnemyWaitState::Update(Enemy& _enemy)
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	int elapsed_frame = mode_game->GetModeCount() - waitstart_frame_;

	//索敵状態であれば、経過時間によって待機と直進を交互に繰り返す
	float random_rot_direction = 1.0f;
	if (elapsed_frame % (60 * (GetRand(5) + 1)) == 0)
	{
		if (_enemy.GetMoveStatus() == enemy::Enemy::MOVE_STATUS::WAIT)
		{
			_enemy.SetMoveStatus(enemy::Enemy::MOVE_STATUS::STRAIGHT);

			//回転方向をランダムに変える
			random_rot_direction *= powf(-1.0f, static_cast<float>(GetRand(1) + 1));
		}
		else
		{
			_enemy.SetMoveStatus(enemy::Enemy::MOVE_STATUS::WAIT);
		}
	}

	//状態によって移動する方向や量を決定する
	VECTOR move = { 0,0,0 };
	VECTOR rotation = _enemy.GetRotation();

	switch (_enemy.GetMoveStatus())
	{
	case Enemy::MOVE_STATUS::WAIT:
		_enemy.SetAnimStatus(Enemy::ANIM_STATUS::WAIT);

		rotation.y += DEG2RAD(_enemy.GetRotationSpeed()) * random_rot_direction;
		break;
	case Enemy::MOVE_STATUS::STRAIGHT:
		_enemy.SetAnimStatus(Enemy::ANIM_STATUS::MOVE);

		move = VScale(VNorm(utility::GetForwardVector(rotation.y)), _enemy.GetWalkSpeed());
		break;
	}

	_enemy.SetRotation(rotation);
	_enemy.Move(move);
}