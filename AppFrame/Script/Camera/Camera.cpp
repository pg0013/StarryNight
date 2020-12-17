/**
 * @file    Camera.cpp
 * @brief  カメラクラス処理
 *
 * @author Takuya Fujisawa
 * @date    202012/11
 */
#include "Camera.h"
#include"../Utility/DefineMacro.h"
#include"../Application/ApplicationBase.h"
#include<cmath>

namespace camera
{
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
		position_ = VGet(0, 90.f, -300.f);
		target_ = VGet(0.0f, 60.0f, 0.0f);
		clip_.near_ = 2.f;
		clip_.far_ = 10000.f;

		move_speed_ = 5.0f;
		rot_speed_ = 0.05f;
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
			if (stick_rx > analog_min)
				camera_rad -= rot_speed_ * stick_rx;
			if (stick_rx < -analog_min)
				camera_rad += rot_speed_ * -stick_rx;

			position_.x = target_.x + length * cos(camera_rad);
			position_.z = target_.z + length * sin(camera_rad);

			if (stick_ry > analog_min)
			{
				position_.y -= move_speed_;
			}
			if (stick_ry < -analog_min)
			{
				position_.y += move_speed_;
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

		DrawFormatString(x, y, DEBUG_COLOR, "%2d Camera",y/DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   |- position	: (%5.2f, %5.2f, %5.2f)", y / DEBUG_FONT_SIZE, position_.x, position_.y, position_.z); y += DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   |- target : (%5.2f, %5.2f, %5.2f)", y / DEBUG_FONT_SIZE, target_.x, target_.y, target_.z); y += DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   |- length : %5.2f", y / DEBUG_FONT_SIZE, length); y += DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   |- rad : % 5.2f", y / DEBUG_FONT_SIZE, camera_rad); y += DEBUG_FONT_SIZE;
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- deg : % 5.2f", y / DEBUG_FONT_SIZE, camera_deg); y += DEBUG_FONT_SIZE;
	}
}