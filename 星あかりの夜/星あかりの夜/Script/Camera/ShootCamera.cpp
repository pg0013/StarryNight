/**
 * @file    ShootCamera.cpp
 * @brief  射撃時のカメラ制御
 *
 * @author Takuya Fujisawa
 * @date   2021/02/05
 */

#include "Camera.h"
#include"CameraShootState.h"
#include"../Stage/Stage.h"
#include"../Mode/ModeGame.h"
#include"../Player/Player.h"
#include<cmath>
using namespace starrynight::camera;

void CameraShootState::SetShootCameraPosition(Camera& _camera)
{
	VECTOR position = _camera.GetPosition();

	//プレイヤーの情報を取得
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
	shoot_camera_pos = VAdd(shoot_camera_pos, VScale(up, -40.0f));
	shoot_camera_pos = VAdd(shoot_camera_pos, VScale(back, 150.0f));

	position = shoot_camera_pos;
	_camera.SetPosition(position);
}

void CameraShootState::SetShootCameraTarget(Camera& _camera)
{
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();
	VECTOR position = _camera.GetPosition();
	VECTOR target = _camera.GetTarget();

	//ターゲットを星空に設定
	VECTOR player_positon = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	player_positon = VAdd(player_positon, VGet(0.0f, 85.0f, 0.0f));
	VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));

	target = VAdd(player_positon, VScale(VNorm(utility::GetForwardVector(player_rotation.y)), 10000));
	target = VAdd(target, VGet(0, 4000, 0));

	float stick_ry;//右アナログスティックの座標

	stick_ry = -x_input.ThumbRY / THUMB_MAX;

	float rot_vertical = 0.0f;

	//y座標を変更するために値を保持
	target.y = old_target_.y;

	//回転軸を設定
	VECTOR rot_vertical_axis = VSub(target, position);
	rot_vertical_axis = VNorm(VCross(rot_vertical_axis, VGet(0, 1, 0)));

	//カメラを上下に移動
	float analog_min = 0.1f;
	if (stick_ry > analog_min)
		rot_vertical -= DEG2RAD(_camera.GetCameraRotSpeed() * 0.3f) * stick_ry;
	if (stick_ry < -analog_min)
		rot_vertical += DEG2RAD(_camera.GetCameraRotSpeed() * 0.3f) * -stick_ry;

	//回転行列を算出
	MATRIX rot_v = MGetRotAxis(rot_vertical_axis, rot_vertical);

	//カメラの右ベクトルを軸に回転
	target.y = VTransform(target, rot_v).y;

	float target_y_min = 0.0f;
	float target_y_max = 10000.0f;

	//カメラ角度の上下限を設定
	if (target.y < target_y_min)
		target.y = target_y_min;
	if (target.y > target_y_max)
		target.y = target_y_max;

	old_target_ = target;

	_camera.SetTarget(target);
}