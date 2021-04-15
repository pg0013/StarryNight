/**
 * @file    Move.cpp
 * @brief	プレイヤー移動処理
 *
 * @author Takuya Fujisawa
 * @date   2020/12/17
 */

#include"PlayerMoveState.h"
#include"Player.h"
#include"../Camera/Camera.h"
#include"../Stage/Stage.h"
using namespace starrynight::player;

void PlayerMoveState::Move(Player& _player)
{
	float camera_rad = camera::Camera::GetInstance()->GetCameraRad();

	//スティックの移動量と角度を計算
	float length = utility::GetLeftStickLength();
	float rad = utility::GetLeftStickRad();

	//スティックの傾ける量で移動速度を変更
	float run_stick_range = 0.6f;

	if (length < run_stick_range)
	{
		length = walk_speed_ * ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();
	}
	else
	{
		length = run_speed_ * ::mode::ModeServer::GetInstance()->Get("Game")->GetDeltaTime();
	}

	move_ = { 0,0,0 };
	VECTOR rotation = _player.GetRotation();

	move_.x = cos(rad + camera_rad) * length;
	move_.z = sin(rad + camera_rad) * length;

	VECTOR old_move = move_;
	float direction = VCross(VNorm(move_), utility::GetForwardVector(rotation.y)).y;

	//move分移動
	position_ = VAdd(position_, move_);

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;

	//腰から地面までの線分ベクトル
	VECTOR start_line = VAdd(position_, VGet(0, 60.0f, 0));
	VECTOR end_line = VAdd(position_, VGet(0, -30.0f, 0));

	//床との当たり判定
	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);
	//踏んでいる床の材質を設定
	_player.SetFloorType(stage::Stage::GetInstance()->GetHitFloorType(start_line, end_line));

	if (hit_poly_floor.HitFlag)
	{
		//キャラクターのy座標を調整
		position_.y = hit_poly_floor.HitPosition.y;
	}

	//回転を設定
	_player.SetRotation(rotation);
}

void PlayerMoveState::SlidingWall(Player& _player)
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

		position_ = old_position_;
		position_ = VAdd(position_, escape);

		//腰から地面までの線分ベクトル
		VECTOR start_line = VAdd(position_, VGet(0, 60.0f, 0));
		VECTOR end_line = VAdd(position_, VGet(0, -30.0f, 0));

		MV1_COLL_RESULT_POLY hit_poly_floor_wallpush = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

		//壁ずり後に床にのめりこむのを修正する
		if (hit_poly_floor_wallpush.HitFlag)
			position_.y = hit_poly_floor_wallpush.HitPosition.y;

		WallPush(_player);
	}

	//当たり判定を解放
	MV1CollResultPolyDimTerminate(hit_poly_wall);
}

void PlayerMoveState::WallPush(Player& _player)
{
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
		{
			MV1CollResultPolyDimTerminate(hit_poly_wallpush);
			break;
		}

		VECTOR normal = { 0,0,0 };
		for (int i = 0; i < hit_poly_wallpush.HitNum; i++)
		{
			normal = VAdd(normal, VNorm(hit_poly_wallpush.Dim[i].Normal));
		}

		//壁に頭がぶつかるとき以外は、押し出しベクトルのy成分は0にする
		if (normal.y > -1.0f)
		{
			normal.y = 0;
		}

		normal = VNorm(normal);

		//押し出し
		position_ = VAdd(position_, VScale(normal, 0.5f));

		MV1CollResultPolyDimTerminate(hit_poly_wallpush);
	}
}

void PlayerMoveState::DecideForwardDirection(Player& _player)
{
	VECTOR rotation = _player.GetRotation();

	//スティックの移動量と角度を計算
	float length = utility::GetLeftStickLength();
	float rad = utility::GetLeftStickRad();
	float camera_rad = camera::Camera::GetInstance()->GetCameraRad();

	VECTOR move = { 0,0,0 };
	move.x = cos(rad + camera_rad) * length;
	move.z = sin(rad + camera_rad) * length;

	if (length > ANALOG_MIN)
	{
		//スティック方向の角度
		float stick_rad = atan2(-1.0f * move.x, -1.0f * move.z);

		//forwardベクトルを求める
		VECTOR forward = utility::GetForwardVector(rotation.y);

		float range = DEG2RAD(10.0f);

		float direction = VCross(VNorm(move), forward).y;
		if (direction > -range && direction < range)
		{
			rotation.y = stick_rad;
		}
		else if (VCross(move, forward).y > 0)
		{
			rotation.y -= DEG2RAD(rot_speed_);
		}
		else if (VCross(move, forward).y < 0)
		{
			rotation.y += DEG2RAD(rot_speed_);
		}
	}

	//プレイヤーの向きを設定
	_player.SetRotation(rotation);
}