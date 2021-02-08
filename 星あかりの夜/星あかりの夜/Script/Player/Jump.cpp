/**
 * @file    Jump.cpp
 * @brief  プレイヤージャンプ処理
 *
 * @author Takuya Fujisawa
 * @date   2021/01/13
 */

#include"Player.h"
#include"../Mode/ModeGame.h"
#include"../Stage/Stage.h"
using namespace starrynight::player;

void Player::Jump()
{
	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	//腰から地面までの線分ベクトル
	VECTOR start_line = VAdd(position_, VGet(0, 40.0f, 0));
	VECTOR end_line = VAdd(position_, VGet(0, -5.0f, 0));

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;
	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

	if (hit_poly_floor.HitFlag)
	{
		if (trigger_key & PAD_INPUT_1)
		{
			jump_speed_ = 15.0f;
			status_ = STATUS::JUMP_START;
			jump_flag_ = true;
		}
		else
		{
			if (jump_flag_)
			{
				if (jump_speed_ < 0.0f)
					jump_flag_ = false;
			}
			else
			{
				jump_flag_ = false;
			}
		}
	}

	if (!hit_poly_floor.HitFlag &&
		jump_flag_ == false)
	{
		jump_flag_ = true;
		jump_speed_ = 0.0f;
	}

	if (jump_flag_)
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

		//移動量の計算
		VECTOR move = { 0,0,0 };
		move.x = cos(rad + camera_rad) * length;
		move.z = sin(rad + camera_rad) * length;

		//腰から地面までの線分ベクトル
		VECTOR start_line = VAdd(position_, VGet(0, 40.0f, 0));
		VECTOR end_line = VAdd(position_, VGet(0, -10.0f, 0));

		//プレイヤーのカプセル情報
		VECTOR capsule_positon1 = VAdd(position_, VGet(0, 100, 0));
		VECTOR capsule_positon2 = VAdd(position_, VGet(0, 45, 0));
		float radius = 35.0f;

		//乗れるオブジェクトとの当たり判定
		MV1_COLL_RESULT_POLY_DIM hit_capsule_wall;
		hit_capsule_wall = stage::Stage::GetInstance()->GetHitCapsuleToWall(capsule_positon1, capsule_positon2, radius);

		if (hit_capsule_wall.HitNum > 0)
		{
			VECTOR normal = { 0,0,0 };
			for (int i = 0; i < hit_capsule_wall.HitNum; i++)
			{
				normal = VAdd(normal, hit_capsule_wall.Dim[i].Normal);
			}
			normal = VNorm(normal);
			VECTOR fall = { 0,jump_speed_,0 };

			move = VAdd(move, fall);

			VECTOR escape = VCross(move, normal);
			escape = VCross(normal, escape);

			position_ = VAdd(position_, escape);

			VECTOR camera_diff = camera_pos;
			camera_diff.x = camera_tar.x + 300.0f * cos(camera_rad);
			camera_diff.z = camera_tar.z + 300.0f * sin(camera_rad);

			camera::Camera::GetInstance()->SetPosition(camera_diff);
			camera::Camera::GetInstance()->SetTarget(VAdd(position_, VGet(0.0f, 60.0f, 0.0f)));

			MV1CollResultPolyDimTerminate(hit_capsule_wall);
		}
		else
		{
			//移動処理
			position_ = VAdd(position_, move);
			position_.y += jump_speed_;
			move.y += jump_speed_;
		}

		//カメラを移動
		camera::Camera::GetInstance()->SetPosition(VAdd(camera_pos, move));
		camera::Camera::GetInstance()->SetTarget(VAdd(position_, VGet(0.0f, 60.0f, 0.0f)));

		jump_speed_ -= gravity_;

		//ジャンプアニメーションを設定
		if (jump_speed_ < 0.0f)
		{
			VECTOR start_line = VAdd(position_, VGet(0, 40.0f, 0));
			VECTOR end_line = VAdd(position_, VGet(0, -40.0f, 0));

			//Navimeshとの当たり判定
			MV1_COLL_RESULT_POLY hit_jump_floor;
			hit_jump_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

			if (hit_jump_floor.HitFlag)
				status_ = STATUS::JUMP_END;
			else
				status_ = STATUS::JUMP_LOOP;
		}
	}
}