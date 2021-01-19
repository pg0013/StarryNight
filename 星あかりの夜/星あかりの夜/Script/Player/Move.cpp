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

	float analog_min = 0.2f;//アナログスティックのデッドスペース
	if (length < analog_min)
		length = 0.0f;
	else
		length = move_speed_ * ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();

	VECTOR move = { 0,0,0 };
	move.x = cos(rad + camera_rad) * length;
	move.z = sin(rad + camera_rad) * length;

	VECTOR old_positon = position_;
	VECTOR old_move = move;

	float direction = VCross(VNorm(move), utility::GetForwardVector(rotation_.y)).y;

	//壁ずり処理
	int escape_max_rad = 80;
	for (int i = 0; i < escape_max_rad; i++)
	{
		float escape_rad = DEG2RAD(i);

		//壁ずりする角度をプレイヤーの向きによって調整
		if (direction > 0)
			escape_rad *= -1;

		//moveの角度をescape_rad分変更
		move.x = cos(rad + camera_rad + escape_rad) * length;
		move.z = sin(rad + camera_rad + escape_rad) * length;

		//move分移動
		position_ = VAdd(position_, move);

		//Navimeshとの当たり判定
		MV1_COLL_RESULT_POLY hit_poly_stage;

		//腰から地面までの線分ベクトル
		VECTOR start_line = VAdd(position_, VGet(0, 40.0f, 0));
		VECTOR end_line = VAdd(position_, VGet(0, -10.0f, 0));

		hit_poly_stage = stage::Stage::GetInstance()->GetHitLineToNaviMesh(start_line, end_line);

		if (hit_poly_stage.HitFlag &&
			jump_flag_ == false)
		{
			position_.y = hit_poly_stage.HitPosition.y;

			//キャラクターのy座標を調整
			move.y += position_.y - old_positon.y;

			//カメラを移動
			camera::Camera::GetInstance()->SetPosition(VAdd(camera_pos, move));
			camera::Camera::GetInstance()->SetTarget(VAdd(position_, VGet(0.0f, 60.0f, 0.0f)));

			break;
		}
		else
		{
			//Navimeshに当たらなかったので元に戻す
			position_ = old_positon;
			move = old_move;
		}
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