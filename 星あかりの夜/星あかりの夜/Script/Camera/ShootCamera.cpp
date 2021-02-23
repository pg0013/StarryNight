/**
 * @file    ShootCamera.cpp
 * @brief  射撃時のカメラ制御
 *
 * @author Takuya Fujisawa
 * @date   2021/02/05
 */
#include "Camera.h"
#include"../Stage/Stage.h"
#include<cmath>
using namespace starrynight::camera;

void Camera::ShootCameraInit()
{
	VECTOR player_positon = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	player_positon = VAdd(player_positon, VGet(0.0f, 85.0f, 0.0f));
	VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));

	//プレイヤーの向きベクトル
	VECTOR right = VNorm(utility::GetRightVector(player_rotation.y));
	VECTOR up = VNorm(utility::GetUpVector(player_rotation.y));
	VECTOR back = VScale(VNorm(utility::GetForwardVector(player_rotation.y)), -1.0f);

	//プレイヤーの右後ろにカメラを設定
	VECTOR shoot_camera_pos;
	shoot_camera_pos = VAdd(player_positon, VScale(right, 80.0f));
	shoot_camera_pos = VAdd(shoot_camera_pos, VScale(up, 10.0f));
	shoot_camera_pos = VAdd(shoot_camera_pos, VScale(back, 150.0f));

	position_ = shoot_camera_pos;

	//ターゲットはプレイヤーの向きに合わせる
	target_ = VAdd(player_positon, VScale(VNorm(utility::GetForwardVector(player_rotation.y)), clip_.far_));
}

void Camera::ShootCamera()
{
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	float stick_ry;//右アナログスティックの座標
	float analog_min = 0.2f;

	stick_ry = -x_input.ThumbRY / THUMB_MAX;

	float rot_vertical = 0.0f;

	VECTOR old_target = target_;
	ShootCameraInit();
	//y座標を変更するために値を保持
	target_.y = old_target.y;

	//回転軸を設定
	VECTOR rot_vertical_axis = VSub(target_, position_);
	rot_vertical_axis = VNorm(VCross(rot_vertical_axis, VGet(0, 1, 0)));

	if (stick_ry > analog_min)
		rot_vertical -= DEG2RAD(rot_speed_) * stick_ry;
	if (stick_ry < -analog_min)
		rot_vertical += DEG2RAD(rot_speed_) * -stick_ry;

	//回転行列を算出
	MATRIX rot_v = MGetRotAxis(rot_vertical_axis, rot_vertical);

	//カメラの右ベクトルを軸に回転
	target_.y = VTransform(target_, rot_v).y;

	float target_y_min = 0.0f;
	float target_y_max = 10000.0f;

	//カメラ角度の上下限を設定
	if (target_.y < target_y_min)
		target_.y = target_y_min;
	if (target_.y > target_y_max)
		target_.y = target_y_max;
}