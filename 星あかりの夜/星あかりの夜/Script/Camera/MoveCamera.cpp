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
	float analog_min = 0.2f;

	stick_rx = x_input.ThumbRX / THUMB_MAX;
	stick_ry = -x_input.ThumbRY / THUMB_MAX;

	float diff_x = position_.x - target_.x;
	float diff_z = position_.z - target_.z;
	float camera_rad = atan2(diff_z, diff_x);
	float length = 300.0f;

	//右スティックカメラ回転
	{
		VECTOR old_position = position_;

		if (stick_rx > analog_min)
			camera_rad -= DEG2RAD(rot_speed_) * stick_rx;
		if (stick_rx < -analog_min)
			camera_rad += DEG2RAD(rot_speed_) * -stick_rx;

		position_.x = target_.x + length * cos(camera_rad);
		position_.z = target_.z + length * sin(camera_rad);

		if (stick_ry < -analog_min)
		{
			position_.y += move_speed_;
			return;
		}

		MV1_COLL_RESULT_POLY hit_stage;
		VECTOR hit_end_line = VAdd(position_, VGet(0, -90, 0));
		hit_stage = stage::Stage::GetInstance()->GetHitLineToFloor(position_, hit_end_line);

		if (stick_ry > analog_min)
		{
			position_.y -= move_speed_;
		}
		if (hit_stage.HitFlag)
		{
			float camera_position_min = hit_stage.HitPosition.y + 80;

			//下りの傾斜でカメラが床に沿って移動しないようにする
			if (old_position.y > camera_position_min)
				position_.y = old_position.y;
			else
				position_.y = camera_position_min;
			return;
		}
	}
}