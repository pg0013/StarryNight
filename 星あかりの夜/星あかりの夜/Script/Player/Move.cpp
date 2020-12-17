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
		length = move_speed_ * ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();

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
		float stick_rot = atan2(-1 * move.x, -1 * move.z);

		//0~360に変更
		if (stick_rot < 0.0f)
			stick_rot += DEG2RAD(360.0f);
		if (rotation_.y < 0.0f)
			rotation_.y += DEG2RAD(360.0f);

		printfDx("rotation : %f\n", RAD2DEG(rotation_.y));
		printfDx("atan2 : %f\n", RAD2DEG(stick_rot));

		if (RAD2DEG(stick_rot) - RAD2DEG(rotation_.y) <= 180.0f)
		{
			rotation_.y += DEG2RAD(3.0f);
			if (rotation_.y > stick_rot) { rotation_.y = stick_rot; }
		}
		else if (RAD2DEG(stick_rot) - RAD2DEG(rotation_.y) >= -180.0f)
		{
			rotation_.y -= DEG2RAD(3.0f);
			//if (rotation_.y < stick_rot) { rotation_.y = stick_rot; }
		}
		else if (RAD2DEG(stick_rot) - RAD2DEG(rotation_.y) < -180.0f)
		{
			rotation_.y -= DEG2RAD(3.0f);
			if (rotation_.y < stick_rot) { rotation_.y = stick_rot; }
		}



	}

}