/**
 * @file    Move.cpp
 * @brief	プレイヤー移動処理
 *
 * @author Takuya Fujisawa
 * @date   2020/12/17
 */

#include"Player.h"
#include"../Mode/ModeGame.h"
#include"../Stage/Stage.h"
using namespace starrynight::player;

void Player::Move()
{
	VECTOR camera_pos = camera::Camera::GetInstance()->GetPosition();
	VECTOR camera_tar = camera::Camera::GetInstance()->GetTarget();
	float camera_rad = camera::Camera::GetInstance()->GetCameraRad();

	//スティックの移動量と角度を計算
	float length = utility::GetLeftStickLength();
	float rad = utility::GetLeftStickRad();

	float analog_min = 0.1f;//アナログスティックのデッドスペース
	if (length < analog_min)
		length = 0.0f;
	else if (length < 0.6f)
		length = walk_speed_ * ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();
	else
		length = run_speed_ * ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();

	VECTOR move = { 0,0,0 };

	//着地硬直
	if (status_ == STATUS::JUMP_END)
	{
		if (length < analog_min)
			move = { 0,0,0 };
		else
			length = 2.5f;
	}

	move.x = cos(rad + camera_rad) * length;
	move.z = sin(rad + camera_rad) * length;

	VECTOR old_position = position_;
	VECTOR old_move = move;
	float direction = VCross(VNorm(move), utility::GetForwardVector(rotation_.y)).y;

	//move分移動
	if (jump_flag_ == false)
	{
		position_ = VAdd(position_, move);
	}

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;

	//腰から地面までの線分ベクトル
	VECTOR start_line = VAdd(position_, VGet(0, 40.0f, 0));
	VECTOR end_line = VAdd(position_, VGet(0, -30.0f, 0));

	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

	//プレイヤーのカプセル情報
	VECTOR capsule_positon1 = VAdd(position_, VGet(0, 100, 0));
	VECTOR capsule_positon2 = VAdd(position_, VGet(0, 45, 0));
	float radius = 35.0f;

	MV1_COLL_RESULT_POLY_DIM hit_poly_wall;
	hit_poly_wall = stage::Stage::GetInstance()->GetHitCapsuleToWall(capsule_positon1, capsule_positon2, radius);

	if (hit_poly_wall.HitNum > 0)
	{
		VECTOR normal = { 0,0,0 };
		for (int i = 0; i < hit_poly_wall.HitNum; i++)
		{
			normal = VAdd(normal, hit_poly_wall.Dim[i].Normal);
		}
		normal = VNorm(normal);

		VECTOR escape = VCross(move, normal);
		escape = VCross(normal, escape);
		escape.y = 0.0f;

		position_ = old_position;
		position_ = VAdd(position_, escape);

		MV1CollResultPolyDimTerminate(hit_poly_wall);

		VECTOR camera_diff = camera_pos;
		camera_diff.x = camera_tar.x + 300.0f * cos(camera_rad);
		camera_diff.z = camera_tar.z + 300.0f * sin(camera_rad);

		camera::Camera::GetInstance()->SetPosition(camera_diff);
		camera::Camera::GetInstance()->SetTarget(VAdd(position_, VGet(0.0f, 60.0f, 0.0f)));

		//押し出し処理
		while (1)
		{
			//プレイヤーのカプセル情報
			VECTOR capsule_positon1 = VAdd(position_, VGet(0, 100, 0));
			VECTOR capsule_positon2 = VAdd(position_, VGet(0, 45, 0));
			float radius = 35.0f;

			MV1_COLL_RESULT_POLY_DIM hit_poly_wallpush;
			hit_poly_wallpush = stage::Stage::GetInstance()->GetHitCapsuleToWall(capsule_positon1, capsule_positon2, radius);

			if (hit_poly_wallpush.HitNum == 0)
				break;

			VECTOR normal = { 0,0,0 };
			for (int i = 0; i < hit_poly_wall.HitNum; i++)
			{
				normal = VAdd(normal, VNorm(hit_poly_wall.Dim[i].Normal));
			}
			normal = VNorm(normal);
			normal.y = 0;

			position_ = VAdd(position_, VScale(normal, 0.5f));

			MV1CollResultPolyDimTerminate(hit_poly_wallpush);
		}
	}
	else if (hit_poly_floor.HitFlag &&
		jump_flag_ == false)
	{
		position_.y = hit_poly_floor.HitPosition.y;

		//キャラクターのy座標を調整
		move.y += position_.y - old_position.y;

		//カメラを移動
		camera::Camera::GetInstance()->SetPosition(VAdd(camera_pos, move));
		camera::Camera::GetInstance()->SetTarget(VAdd(position_, VGet(0.0f, 60.0f, 0.0f)));
	}

	//アニメーションステータスを設定
	if (jump_flag_ == false)
	{
		if (status_ == STATUS::JUMP_END &&
			anim_play_time_ < anim_total_time_ * 2 / 3)
			status_ = STATUS::JUMP_END;
		else if (utility::GetLeftStickLength() < analog_min)
			status_ = STATUS::WAIT;
		else if (utility::GetLeftStickLength() < 0.6f)
			status_ = STATUS::WALK;
		else
			status_ = STATUS::RUN;
	}

	if (length > analog_min)
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
		else if (VCross(move, forward).y > 0)
		{
			rotation_.y -= DEG2RAD(rot_speed_);
		}
		else if (VCross(move, forward).y < 0)
		{
			rotation_.y += DEG2RAD(rot_speed_);
		}
	}
}