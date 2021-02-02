/**
 * @file    Camera.cpp
 * @brief  カメラクラス処理
 *
 * @author Takuya Fujisawa
 * @date    202012/11
 */
#include "Camera.h"
#include"../Stage/Stage.h"
#include<cmath>
using namespace starrynight::camera;

Camera* Camera::instance_ = nullptr;

Camera::Camera()
{
	instance_ = this;
}

Camera::~Camera()
{
	instance_ = nullptr;
}

void Camera::Initialize()
{
	std::vector<std::string> param = { "move_speed","rot_speed","near","far" };
	camera_param_.LoadCameraParameters(param);

	position_ = VGet(0, 90.f, -300.f);
	target_ = VGet(0.0f, 60.0f, 0.0f);
	clip_.near_ = camera_param_.GetCameraParam("near");
	clip_.far_ = camera_param_.GetCameraParam("far");

	move_speed_ = camera_param_.GetCameraParam("move_speed");
	rot_speed_ = camera_param_.GetCameraParam("rot_speed");
}

void Camera::Input()
{
}

void Camera::Process()
{
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	float stick_rx, stick_ry;//右アナログスティックの座標
	float analog_min = 0.2f;

	stick_rx = x_input.ThumbRX / THUMB_MAX;
	stick_ry = -x_input.ThumbRY / THUMB_MAX;

	float diff_x = position_.x - target_.x;
	float diff_z = position_.z - target_.z;
	float camera_rad = atan2(diff_z, diff_x);
	float length = sqrt(diff_z * diff_z + diff_x * diff_x);

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

void Camera::Render()
{
	SetCameraPositionAndTarget_UpVecY(position_, target_);
	SetCameraNearFar(clip_.near_, clip_.far_);
}

void Camera::DrawDebugMenu()
{
	int x, y;
	x = 0; y = DEBUG_FONT_SIZE;

	float diff_x = position_.x - target_.x;
	float diff_z = position_.z - target_.z;
	float camera_rad = atan2(diff_z, diff_x);
	float camera_deg = RAD2DEG(camera_rad);
	float length = sqrt(diff_z * diff_z + diff_x * diff_x);

	DrawFormatString(x, y, DEBUG_COLOR, "%2d Camera", y / DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   |- position	: (%5.2f, %5.2f, %5.2f)", y / DEBUG_FONT_SIZE, position_.x, position_.y, position_.z); y += DEBUG_FONT_SIZE;
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   |- target : (%5.2f, %5.2f, %5.2f)", y / DEBUG_FONT_SIZE, target_.x, target_.y, target_.z); y += DEBUG_FONT_SIZE;
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   |- length : %5.2f", y / DEBUG_FONT_SIZE, length); y += DEBUG_FONT_SIZE;
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   |- rad : % 5.2f", y / DEBUG_FONT_SIZE, camera_rad); y += DEBUG_FONT_SIZE;
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- deg : % 5.2f", y / DEBUG_FONT_SIZE, camera_deg); y += DEBUG_FONT_SIZE;
}

float Camera::GetCameraRad()
{
	float diff_x = position_.x - target_.x;
	float diff_z = position_.z - target_.z;

	float camera_rad = atan2(diff_z, diff_x);
	return camera_rad;
}

float Camera::GetCameraLength()
{
	float diff_x = position_.x - target_.x;
	float diff_z = position_.z - target_.z;

	float length = sqrt(diff_z * diff_z + diff_x * diff_x);
	return length;
}