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

	//ジャンプ開始処理
	if (hit_poly_floor.HitFlag)
	{
		if (trigger_key & PAD_INPUT_2)
		{
			//床に接地している状態でAボタンが押されたら、ジャンプを開始する
			jump_speed_ = jump_height_;
			status_ = STATUS::JUMP_START;
			jump_flag_ = true;
		}
		else
		{
			//ジャンプや落下後に地面に着地したら、ジャンプ状態を解除する
			if (jump_flag_)
			{
				if (jump_speed_ < 0.0f)
					jump_flag_ = false;
			}
		}
	}

	//崖などから飛び降りたら落下状態にして、ジャンプ中に設定する
	if (!hit_poly_floor.HitFlag &&
		jump_flag_ == false)
	{
		jump_flag_ = true;
		jump_speed_ = 0.0f;
	}

	//ジャンプ中の処理
	if (jump_flag_)
	{
		VECTOR camera_pos = camera::Camera::GetInstance()->GetPosition();
		VECTOR camera_tar = camera::Camera::GetInstance()->GetTarget();
		float camera_rad = camera::Camera::GetInstance()->GetCameraRad();

		//スティックの移動量と角度を計算
		float length = utility::GetLeftStickLength();
		float rad = utility::GetLeftStickRad();

		//スティックの傾ける量で移動速度を変更
		if (length < ANALOG_MIN)
			length = 0.0f;
		else if (length < 0.6f)
			length = walk_speed_ * ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();
		else
			length = run_speed_ * ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();

		//移動量の計算
		VECTOR move = { 0,0,0 };
		move.x = cos(rad + camera_rad) * length;
		move.z = sin(rad + camera_rad) * length;

		//移動処理
		position_ = VAdd(position_, move);
		position_.y += jump_speed_;
		move.y += jump_speed_;

		//ジャンプ加速処理
		jump_speed_ -= gravity_;
	}
}