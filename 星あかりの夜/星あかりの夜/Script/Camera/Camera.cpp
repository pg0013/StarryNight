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
	//パラメータをjsonから読み込み
	std::vector<std::string> param = { "move_speed","rot_speed","near","far","camera_distance" };
	camera_param_.LoadCameraParameters(param);

	//カメラパラメータ初期化
	move_speed_ = camera_param_.GetCameraParam("move_speed");
	rot_speed_ = camera_param_.GetCameraParam("rot_speed");
	camera_distance_ = camera_param_.GetCameraParam("camera_distance");

	//カメラ設定を初期化
	position_ = VGet(0, 90.f, -1.0f * camera_distance_);
	target_ = VAdd(VGet(0.0f, 60.0f, 0.0f), MV1GetPosition(resource::ResourceServer::GetModelHandle("player")));
	clip_.near_ = camera_param_.GetCameraParam("near");
	clip_.far_ = camera_param_.GetCameraParam("far");
	status_ = STATUS::MOVE;
}

void Camera::Input()
{
}

void Camera::Process()
{
	switch (status_)
	{
	case STATUS::MOVE:
		//移動用カメラ処理
		MoveCamera();
		break;
	case STATUS::SHOOT:
		//射撃用カメラ処理
		ShootCamera();
		break;
	}
}

void Camera::Render()
{
	SetCameraPositionAndTarget_UpVecY(position_, target_);
	SetCameraNearFar(clip_.near_, clip_.far_);
}

void Camera::DrawDebugMenu()
{
#ifdef DEBUG_FUNCTION
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
#endif
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