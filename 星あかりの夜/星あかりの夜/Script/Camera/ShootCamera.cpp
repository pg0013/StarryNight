/**
 * @file    ShootCamera.cpp
 * @brief  射撃時のカメラ制御
 *
 * @author Takuya Fujisawa
 * @date   2021/02/05
 */

#include "Camera.h"
#include"../Stage/Stage.h"
#include"../Mode/ModeGame.h"
#include"../Player/Player.h"
#include<cmath>
using namespace starrynight::camera;

void Camera::ShootCameraInit()
{
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

	position_ = shoot_camera_pos;

	//ターゲットを星空に設定
	target_ = VAdd(player_positon, VScale(VNorm(utility::GetForwardVector(player_rotation.y)), 10000));
	target_ = VAdd(target_, VGet(0, 4000, 0));

	target_.y = old_target_.y;
}

void Camera::ShootCamera()
{
	//カメラの位置とターゲットを初期化
	//カメラの左右移動はプレイヤーの向きによって、移動処理を行う
	ShootCameraInit();

	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	//プレイヤーが星を選択したかどうかを取得
	bool selected_star_flag = false;
	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if (iter->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(iter);
			selected_star_flag = player->GetSelectedStarFlag();
			break;
		}
	}

	//星選択済みならカメラ移動しない
	if (selected_star_flag == true)
		return;

	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	float stick_ry;//右アナログスティックの座標

	stick_ry = -x_input.ThumbRY / THUMB_MAX;

	float rot_vertical = 0.0f;

	//y座標を変更するために値を保持
	target_.y = old_target_.y;

	//回転軸を設定
	VECTOR rot_vertical_axis = VSub(target_, position_);
	rot_vertical_axis = VNorm(VCross(rot_vertical_axis, VGet(0, 1, 0)));

	//カメラを上下に移動
	float analog_min = 0.1f;
	if (stick_ry > analog_min)
		rot_vertical -= DEG2RAD(rot_speed_ * 0.3f) * stick_ry;
	if (stick_ry < -analog_min)
		rot_vertical += DEG2RAD(rot_speed_ * 0.3f) * -stick_ry;

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

	old_target_ = target_;
}