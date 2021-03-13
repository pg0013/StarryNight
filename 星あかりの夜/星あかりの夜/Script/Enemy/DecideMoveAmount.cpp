/**
 * @file    DecideMoveAmount.cpp
 * @brief  エネミーの移動量や方向を、MOVE_STATUSから決定する処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include "Enemy.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::enemy;

VECTOR Enemy::DecideMoveAmount()
{
	VECTOR move = { 0,0,0 };
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	float player_distance = VSize(VSub(player_position, position_));

	//状態によって移動する方向や量を決定する
	switch (move_status_)
	{
	case MOVE_STATUS::WAIT:
		anim_status_ = ANIM_STATUS::WAIT;

		rotation_.y += DEG2RAD(rot_speed_) * random_rot_direction_;
		break;
	case MOVE_STATUS::STRAIGHT:
		anim_status_ = ANIM_STATUS::MOVE;

		move = VScale(VNorm(utility::GetForwardVector(rotation_.y)), walk_speed_);
		break;
	case MOVE_STATUS::TRACKING:
		anim_status_ = ANIM_STATUS::RUN;

		move = Tracking(move);
		break;
	case MOVE_STATUS::ATTACK:
		anim_status_ = ANIM_STATUS::ATTACK;

		move = Attack(move);
		break;
	case MOVE_STATUS::ESCAPE:
		anim_status_ = ANIM_STATUS::MOVE;

		if (player_distance > detect_length_ + 300.0f)
			move_status_ = MOVE_STATUS::WAIT;
		else
			move = VScale(VNorm(utility::GetForwardVector(rotation_.y)), run_speed_);
		break;
	}
	return move;
}