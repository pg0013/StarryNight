/**
 * @file    Jump.cpp
 * @brief  プレイヤージャンプ処理
 *
 * @author Takuya Fujisawa
 * @date   2021/01/13
 */

#include"PlayerJumpState.h"
#include"Player.h"
#include"../Camera/Camera.h"
#include"../Stage/Stage.h"
using namespace starrynight::player;

void PlayerJumpState::Move(Player& _player)
{
	//スティックの移動量と角度を計算
	float camera_rad = camera::Camera::GetInstance()->GetCameraRad();
	float length = utility::GetLeftStickLength();
	float rad = utility::GetLeftStickRad();

	//スティックの傾ける量で移動速度を変更
	float run_stick_range = 0.6f;

	if (length < ANALOG_MIN)
	{
		length = 0.0f;
	}
	else 	if (length < run_stick_range)
	{
		length = walk_speed_ * ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();
	}
	else
	{
		length = run_speed_ * ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();
	}

	//着地硬直
	if (_player.GetPlayerStatus() == Player::STATUS::JUMP_END &&
		_player.GetAnimPlayTime() < _player.GetAnimTotalTime() * 2.0f / 3.0f)
	{
		if (length < ANALOG_MIN)
		{
			move_ = { 0,0,0 };
		}
		else
		{
			float landing_speed = 2.5f;
			length = landing_speed;
		}
	}

	//移動量の計算
	move_ = { 0,0,0 };
	move_.x = cos(rad + camera_rad) * length;
	move_.z = sin(rad + camera_rad) * length;

	//移動処理
	position_ = VAdd(position_, move_);
}

void PlayerJumpState::Jump(Player& _player)
{
	position_.y += jump_speed_;
	move_.y += jump_speed_;

	//ジャンプ加速処理
	jump_speed_ -= gravity_;
}

void PlayerJumpState::Landing(Player& _player)
{
	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;

	//腰から地面までの線分ベクトル
	VECTOR start_line = VAdd(position_, VGet(0, 60.0f, 0));
	VECTOR end_line = VAdd(position_, VGet(0, -10.0f, 0));

	//床との当たり判定
	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);
	//踏んでいる床の材質を設定
	_player.SetFloorType(stage::Stage::GetInstance()->GetHitFloorType(start_line, end_line));

	//落下後に接地したら、地面にY座標を合わせる
	if (hit_poly_floor.HitFlag &&
		jump_speed_ < 0)
	{
		//キャラクターのy座標を調整
		position_.y = hit_poly_floor.HitPosition.y;
		jump_speed_ = 0.0f;
	}
}

void PlayerJumpState::SlidingWall(Player& _player)
{
	//プレイヤーのカプセル情報
	VECTOR capsule_positon1 = VAdd(position_, VGet(0, 100, 0));
	VECTOR capsule_positon2 = VAdd(position_, VGet(0, 45, 0));
	float radius = 35.0f;

	//壁との当たり判定を取得
	MV1_COLL_RESULT_POLY_DIM hit_poly_wall;
	hit_poly_wall = stage::Stage::GetInstance()->GetHitCapsuleToWall(capsule_positon1, capsule_positon2, radius);

	//壁ずり処理
	if (hit_poly_wall.HitNum > 0)
	{
		VECTOR normal = { 0,0,0 };
		for (int i = 0; i < hit_poly_wall.HitNum; i++)
		{
			normal = VAdd(normal, hit_poly_wall.Dim[i].Normal);
		}
		normal = VNorm(normal);

		//壁ずりベクトルの算出
		VECTOR escape = VCross(move_, normal);
		escape = VCross(normal, escape);
		escape.y = 0.0f;

		//ジャンプ中は壁刷りによる移動成分の増加を小さくする
		escape = VNorm(escape);

		//position_ = old_position_;
		position_ = VAdd(position_, escape);

		WallPush(_player);
	}

	//当たり判定を解放
	MV1CollResultPolyDimTerminate(hit_poly_wall);
}