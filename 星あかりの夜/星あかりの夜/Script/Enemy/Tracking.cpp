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

VECTOR Enemy::Tracking(VECTOR& _move)
{
	//プレイヤーを発見してから、ワンモーションは移動しない
	if (::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - track_start_frame_ < 25)
	{
		if (::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - track_start_frame_ == 1)
		{
			//プレイヤー発見SEを再生
			attention_se_.Load("Resource/sound/enemy_se3.wav");
			//スクリーン上の位置に対して、左右バランスを設定
			attention_se_.Pan(static_cast<int>(utility::GetScreenPosFromWorldPos(position_).x));
			attention_se_.Play();
		}

		//移動しないでその場で足踏み
		_move = { 0,0,0 };
		return _move;
	}

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	float player_distance = VSize(VSub(player_position, position_));

	VECTOR enemy_to_player = VSub(player_position, position_);
	VECTOR forward = utility::GetForwardVector(rotation_.y);

	VECTOR cross = VCross(forward, enemy_to_player);

	//プレイヤーがエネミーに対して左右どちらかにいるか判定する
	if (VCross(VNorm(forward), VNorm(enemy_to_player)).y >= 0)
	{
		rotation_.y += DEG2RAD(rot_speed_) * VSize(VCross(forward, enemy_to_player)) * 0.02f;
	}
	else
	{
		rotation_.y -= DEG2RAD(rot_speed_) * VSize(VCross(forward, enemy_to_player)) * 0.02f;
	}

	_move = VScale(VNorm(utility::GetForwardVector(rotation_.y)), run_speed_);

	//プレイヤー検出範囲を超えたら追うのをやめる
	if (player_distance > detect_length_)
	{
		move_status_ = MOVE_STATUS::WAIT;
		_move = { 0,0,0 };
	}

	return _move;
}