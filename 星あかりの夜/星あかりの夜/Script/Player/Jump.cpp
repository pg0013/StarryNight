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

	if (trigger_key & PAD_INPUT_1 &&
		jump_flag_ == false)
	{
		jump_speed_ = 15.0f;
		status_ = STATUS::JUMP;
		jump_flag_ = true;
	}

	if (jump_flag_)
	{
		VECTOR camera_pos = camera::Camera::GetInstance()->GetPosition();
		VECTOR camera_tar = camera::Camera::GetInstance()->GetTarget();
		float camera_rad = camera::Camera::GetInstance()->GetCameraRad();

		//スティックの移動量と角度を計算
		float length = utility::GetLeftStickLength();
		float rad = utility::GetLeftStickRad();

		float analog_min = 0.2f;//アナログスティックのデッドスペース
		if (length < analog_min)
			length = 0.0f;
		else
			length = move_speed_ * ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();

		VECTOR move = { 0,0,0 };
		move.x = cos(rad + camera_rad) * length;
		move.z = sin(rad + camera_rad) * length;

		//腰から地面までの線分ベクトル
		VECTOR start_line = VAdd(position_, VGet(0, 40.0f, 0));
		VECTOR end_line = VAdd(position_, VGet(0, -10.0f, 0));

		//乗れるオブジェクトとの当たり判定
		MV1_COLL_RESULT_POLY hit_poly_object;
		hit_poly_object = stage::Stage::GetInstance()->GetHitToColObject(start_line, end_line);

		if (hit_poly_object.HitFlag)
		{
			position_.y = hit_poly_object.HitPosition.y;
			jump_speed_ = 0.0f;

			//足場からジャンプ
			if (trigger_key & PAD_INPUT_1)
				jump_speed_ = 15.0f;
		}

		//頭から前方までの線分ベクトル
		VECTOR start_forward = VAdd(position_, VGet(0, 100.0f, 0));
		VECTOR end_forward = VAdd(start_forward, VScale(VNorm(utility::GetForwardVector(rotation_.y)), 30.0f));

		//乗れるオブジェクトとの当たり判定
		MV1_COLL_RESULT_POLY hit_forward_object;
		hit_forward_object = stage::Stage::GetInstance()->GetHitToColObject(start_forward, end_forward);

		VECTOR old_position = position_;

		if (hit_forward_object.HitFlag)
		{
			position_.x = old_position.x;
			position_.z = old_position.z;
			position_.y += jump_speed_;
			move.y += jump_speed_;
		}
		else
		{
			position_ = VAdd(position_, move);
			position_.y += jump_speed_;
			move.y += jump_speed_;
		}

		//カメラを移動
		camera::Camera::GetInstance()->SetPosition(VAdd(camera_pos, move));
		camera::Camera::GetInstance()->SetTarget(VAdd(position_, VGet(0.0f, 60.0f, 0.0f)));

		jump_speed_ -= gravity_;

		//腰から地面までの線分ベクトル
		start_line = VAdd(position_, VGet(0, 40.0f, 0));
		end_line = VAdd(position_, VGet(0, -10.0f, 0));

		//Navimeshとの当たり判定
		MV1_COLL_RESULT_POLY hit_poly_stage;
		hit_poly_stage = stage::Stage::GetInstance()->GetHitToNaviMesh(start_line, end_line);

		if (hit_poly_stage.HitFlag)
		{
			jump_flag_ = false;
			status_ = STATUS::WAIT;
		}
	}
}