/**
 * @file    EnemyWait.cpp
 * @brief  敵の待機状態処理
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include"EnemyTrackingState.h"
#include"Enemy.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::enemy;

void EnemyTrackingState::Tracking(Enemy& _enemy)
{
	VECTOR move = { 0,0,0 };

	//プレイヤーを発見してから、ワンモーションは移動しない
	int stop_frame = 25;
	if (::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - track_start_frame_ < stop_frame)
	{
		//移動しないでその場で足踏み
		move = { 0,0,0 };
		_enemy.Move(move);

		return;
	}

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR position = _enemy.GetPosition();
	VECTOR rotation = _enemy.GetRotation();

	float player_distance = VSize(VSub(player_position, position));

	VECTOR enemy_to_player = VSub(player_position, position);
	VECTOR forward = utility::GetForwardVector(rotation.y);

	VECTOR cross = VCross(forward, enemy_to_player);

	//プレイヤーがエネミーに対して左右どちらかにいるか判定する
	if (VCross(VNorm(forward), VNorm(enemy_to_player)).y >= 0)
	{
		rotation.y += DEG2RAD(_enemy.GetRotationSpeed()) * VSize(VCross(forward, enemy_to_player)) * 0.02f;
	}
	else
	{
		rotation.y -= DEG2RAD(_enemy.GetRotationSpeed()) * VSize(VCross(forward, enemy_to_player)) * 0.02f;
	}

	move = VScale(VNorm(utility::GetForwardVector(rotation.y)), _enemy.GetRunSpeed());

	_enemy.SetRotation(rotation);
	_enemy.Move(move);
}