/**
 * @file    Move.cpp
 * @brief	プレイヤー移動処理
 *
 * @author Takuya Fujisawa
 * @date   2020/12/17
 */

#include"Player.h"
#include"../Mode/ModeGame.h"
using namespace starrynight::player;

void Player::Move()
{
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	float stick_lx, stick_ly;//アナログスティックの座標
	float analog_min = 0.2f;//アナログスティックのデッドスペース

	stick_lx = x_input.ThumbLX / THUMB_MAX;
	stick_ly = -x_input.ThumbLY / THUMB_MAX;

	VECTOR camera_pos = camera::Camera::GetInstance()->GetPosition();
	VECTOR camera_tar = camera::Camera::GetInstance()->GetTarget();

	//プレイヤーとの距離と角度を計算
	float diff_x = camera_pos.x - camera_tar.x;
	float diff_z = camera_pos.z - camera_tar.z;
	float camera_rad = atan2(diff_z, diff_x);

	//スティックの移動量と角度を計算
	float length = sqrt(stick_lx * stick_lx + stick_ly * stick_ly);
	float rad = atan2(stick_lx, stick_ly);

	if (length < analog_min)
		length = 0.0f;
	else
		length = move_speed_* ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();

	VECTOR move = { 0,0,0 };
	move.x = cos(rad + camera_rad) * length;
	move.z = sin(rad + camera_rad) * length;

	VECTOR old_positon = position_;
	position_ = VAdd(position_, move);

	MV1_COLL_RESULT_POLY hit_poly;

	mode::ModeGame* game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	hit_poly = MV1CollCheck_Line(
		game->stage_.GetFieldHandle(),
		MV1SearchFrame(game->stage_.GetFieldHandle(), "field_B_NavMesh_GEO"),
		VAdd(position_, VGet(0, 40.0f, 0)), VAdd(position_, VGet(0, -9999.0f, 0)));

	if (hit_poly.HitFlag)
	{
		position_.y = hit_poly.HitPosition.y;

		move.y += position_.y - old_positon.y;

		camera::Camera::GetInstance()->SetPosition(VAdd(camera_pos, move));
		camera::Camera::GetInstance()->SetTarget(VAdd(camera_tar, move));
	}
	else
	{
		position_ = old_positon;
	}

	if (VSize(move) > 0.0f)
	{
		status_ = STATUS::WALK;
	}
	else
	{
		status_ = STATUS::WAIT;
	}

	if (VSize(move) > 0.0f)
	{
		//スティック方向の角度
		float stick_rad = atan2(-1.0f * move.x, -1.0f * move.z);

		//forwardベクトルを求める
		VECTOR forward = utility::GetForwardVector(rotation_.y);

		float range = DEG2RAD(10.0f);

		float direction = VCross(VNorm(move), forward).y;

		if (direction > -range && direction < range)
		{
			rotation_.y = stick_rad;
		}
		else if (VCross(move,forward).y > 0)
		{
			rotation_.y -= DEG2RAD(rot_speed_);
		}
		else if (VCross(move,forward).y < 0)
		{
			rotation_.y += DEG2RAD(rot_speed_);
		}
	}
}