/**
 * @file    Tracking.cpp
 * @brief  エネミーがプレイヤーを追跡する処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include "Enemy.h"
#include"../Mode/ModeGame.h"
#include"../Player/Player.h"

using namespace starrynight::enemy;

VECTOR Enemy::Tracking(VECTOR _move)
{
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	float player_distance = VSize(VSub(player_position, position_));

	VECTOR enemy_to_player = VSub(player_position, position_);
	VECTOR forward = utility::GetForwardVector(rotation_.y);

	VECTOR cross = VCross(forward, enemy_to_player);

	if (VCross(VNorm(forward), VNorm(enemy_to_player)).y >= 0)
	{
		rotation_.y += DEG2RAD(rot_speed_) * VSize(VCross(forward, enemy_to_player)) * 0.02f;
	}
	else
	{
		rotation_.y -= DEG2RAD(rot_speed_) * VSize(VCross(forward, enemy_to_player)) * 0.02f;
	}

	_move = VScale(VNorm(utility::GetForwardVector(rotation_.y)), run_speed_);

	if (player_distance > detect_length_)
	{
		move_status_ = MOVE_STATUS::WAIT;
		_move = { 0,0,0 };
	}

	return _move;
}