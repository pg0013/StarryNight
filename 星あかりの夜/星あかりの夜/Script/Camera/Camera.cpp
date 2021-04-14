/**
 * @file    Camera.cpp
 * @brief  カメラクラス処理
 *
 * @author Takuya Fujisawa
 * @date    202012/11
 */
#include "Camera.h"
#include"CameraMoveState.h"
#include"CameraShootState.h"
#include"CameraSkyStarState.h"
#include"../Stage/Stage.h"
#include<cmath>
using namespace starrynight::camera;

Camera* Camera::instance_ = nullptr;

Camera::Camera()
{
	instance_ = this;
	state_map_.clear();
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

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));

	//カメラ設定を初期化
	target_ = VAdd(VGet(0.0f, 60.0f, 0.0f), player_position);
	position_ = VAdd(VAdd(player_position, VGet(0, 90, 0)),
		VScale(utility::GetForwardVector(player_rotation.y), camera_distance_));
	clip_.near_ = camera_param_.GetCameraParam("near");
	clip_.far_ = camera_param_.GetCameraParam("far");

	status_ = STATUS::MOVE;
	camera_state_ = std::make_shared<CameraMoveState>();
	//状態ポインタをマップに登録
	state_map_.emplace("Move", camera_state_);
	state_map_.emplace("Shoot", std::make_shared<CameraShootState>());
	state_map_.emplace("SkyStar", std::make_shared<CameraSkyStarState>());

}

void Camera::Process()
{
	//状態の切り替え処理
	camera_state_->Input(*this);
	//現在の状態の更新処理
	camera_state_->Update(*this);
}

void Camera::ChangeCameraState(const std::string& _state_name)
{
	auto iter = state_map_.find(_state_name);
	if (iter != state_map_.end())
	{
		//現在の状態の終了処理を実行
		camera_state_->Exit(*this);

		//現在の状態から次の状態へ遷移
		camera_state_ = iter->second;

		//移行した状態の初期化処理を実行
		camera_state_->Enter(*this);
	}
}

void Camera::Render()
{
	//カメラの位置とターゲットを設定
	SetCameraPositionAndTarget_UpVecY(position_, target_);
	//描画距離を設定
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

float Camera::GetCameraRad() const
{
	float diff_x = position_.x - target_.x;
	float diff_z = position_.z - target_.z;

	float camera_rad = atan2(diff_z, diff_x);
	return camera_rad;
}

float Camera::GetCameraLength() const
{
	float diff_x = position_.x - target_.x;
	float diff_z = position_.z - target_.z;

	float length = sqrt(diff_z * diff_z + diff_x * diff_x);
	return length;
}