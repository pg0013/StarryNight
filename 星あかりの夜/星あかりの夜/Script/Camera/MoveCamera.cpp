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

	if (stick_ry < -ANALOG_MIN)
	{
		position_.y += move_speed_;

		return;
	}

	//カメラの高さを下に移動
	if (stick_ry > ANALOG_MIN)
	{
		position_.y -= move_speed_;
	}

	if (x_input.LeftTrigger == 255)
	{
		camera_distance_ -= move_speed_;
	}
	if (x_input.RightTrigger == 255)
	{
		camera_distance_ += move_speed_;
	}


	//カメラの左右位置を更新
	position_.x = target_.x + camera_distance_ * cos(camera_rad);
	position_.z = target_.z + camera_distance_ * sin(camera_rad);

	//カメラを上に移動する

}