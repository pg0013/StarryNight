/**
 * @file    MoveCamera.cpp
 * @brief  移動時のカメラ制御処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/05
 */
#include "Camera.h"
#include"../Stage/Stage.h"
#include<cmath>
using namespace starrynight::camera;

void Camera::MoveCamera()
{
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	float stick_rx, stick_ry;//右アナログスティックの座標

	//スティックの傾きを取得
	stick_rx = x_input.ThumbRX / THUMB_MAX;
	stick_ry = -x_input.ThumbRY / THUMB_MAX;

	//カメラ角度を取得
	float camera_rad = GetCameraRad();

	//右スティックカメラ回転
	VECTOR old_position = position_;

	//カメラを左右に回転する
	if (stick_rx > ANALOG_MIN)
		camera_rad -= DEG2RAD(rot_speed_) * stick_rx;
	if (stick_rx < -ANALOG_MIN)
		camera_rad += DEG2RAD(rot_speed_) * -stick_rx;

	//カメラの左右位置を更新
	position_.x = target_.x + camera_distance_ * cos(camera_rad);
	position_.z = target_.z + camera_distance_ * sin(camera_rad);

	//カメラと壁の押し出し処理
	while (1)
	{
		//プレイヤーのカプセル情報
		VECTOR sphere_position = position_;
		float radius = 60.0f;

		MV1_COLL_RESULT_POLY_DIM hit_poly_wallpush;
		hit_poly_wallpush = stage::Stage::GetInstance()->GetHitSphereToWall(sphere_position, radius);

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
		normal = VNorm(normal);

		position_ = VAdd(position_, VScale(normal, 0.5f));

		MV1CollResultPolyDimTerminate(hit_poly_wallpush);
	}

	//カメラを上に移動する
	if (stick_ry < -ANALOG_MIN)
	{
		position_.y += move_speed_;

		//カメラの高さの上限を設定
		float camera_max_y = MV1GetPosition(resource::ResourceServer::GetModelHandle("player")).y + 300.0f;
		if (position_.y > camera_max_y)
			position_.y = camera_max_y;

		return;
	}

	//ステージの床との当たり判定
	MV1_COLL_RESULT_POLY hit_stage;
	VECTOR hit_end_line = VAdd(position_, VGet(0, -90, 0));
	hit_stage = stage::Stage::GetInstance()->GetHitLineToFloor(position_, hit_end_line);

	//カメラの高さを下に移動
	if (stick_ry > ANALOG_MIN)
	{
		position_.y -= move_speed_;
	}

	//床との当たり判定を検出
	if (hit_stage.HitFlag)
	{
		float camera_position_min = hit_stage.HitPosition.y + 80;

		//床よりカメラが下に行かない
		//下りの傾斜でカメラが床に沿って移動しない
		if (old_position.y > camera_position_min)
			position_.y = old_position.y;
		else
			position_.y = camera_position_min;

		return;
	}
	else
	{
		float camera_y_min = 80;
		if (position_.y < camera_y_min)
			position_.y = camera_y_min;
	}
}