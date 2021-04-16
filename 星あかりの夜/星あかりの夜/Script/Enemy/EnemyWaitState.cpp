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
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();
	waitstart_frame_ = mode_game->GetModeCount();
}

void EnemyWaitState::Exit(Enemy& _enemy)
{
}

void EnemyWaitState::Input(Enemy& _enemy)
{
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	//プレイヤーがダメージを受けたかどうか取得
	bool player_damaged = false;
	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			std::shared_ptr<player::Player> player = std::dynamic_pointer_cast<player::Player>(iter);
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
	Wait(_enemy);
}