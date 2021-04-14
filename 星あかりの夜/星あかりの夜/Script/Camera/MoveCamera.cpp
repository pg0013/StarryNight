/**
 * @file    MoveCamera.cpp
 * @brief  移動時のカメラ制御処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/05
 */
#include"CameraMoveState.h"
#include "Camera.h"
#include"../Stage/Stage.h"
#include<cmath>
using namespace starrynight::camera;

void CameraMoveState::MoveCamera(Camera& _camera)
{
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	VECTOR position = _camera.GetPosition();
	VECTOR target = _camera.GetTarget();

	float stick_rx, stick_ry;//右アナログスティックの座標

	//スティックの傾きを取得
	stick_rx = x_input.ThumbRX / THUMB_MAX;
	stick_ry = -x_input.ThumbRY / THUMB_MAX;

	//カメラ角度を取得
	float camera_rad = _camera.GetCameraRad();

	//右スティックカメラ回転
	VECTOR old_position = position;

	//カメラを左右に回転する
	if (stick_rx > ANALOG_MIN)
		camera_rad -= DEG2RAD(_camera.GetCameraRotSpeed()) * stick_rx;
	if (stick_rx < -ANALOG_MIN)
		camera_rad += DEG2RAD(_camera.GetCameraRotSpeed()) * -stick_rx;

	//カメラの左右位置を更新
	position.x = target.x + _camera.GetCameraDistance() * cos(camera_rad);
	position.z = target.z + _camera.GetCameraDistance() * sin(camera_rad);

	//カメラと壁の押し出し処理
	while (1)
	{
		//プレイヤーのカプセル情報
		VECTOR sphere_position = position;
		float radius = 60.0f;

		//壁との当たり判定をとる
		MV1_COLL_RESULT_POLY_DIM hit_poly_wallpush;
		hit_poly_wallpush = stage::Stage::GetInstance()->GetHitSphereToWall(sphere_position, radius);

		//判定がなければ終了
		if (hit_poly_wallpush.HitNum == 0)
		{
			MV1CollResultPolyDimTerminate(hit_poly_wallpush);
			break;
		}

		//法線ベクトルを算出
		VECTOR normal = { 0,0,0 };
		for (int i = 0; i < hit_poly_wallpush.HitNum; i++)
		{
			normal = VAdd(normal, VNorm(hit_poly_wallpush.Dim[i].Normal));
		}
		normal = VNorm(normal);

		//押し出し
		position = VAdd(position, VScale(normal, 0.5f));

		MV1CollResultPolyDimTerminate(hit_poly_wallpush);
	}

	//カメラを上に移動する
	if (stick_ry < -ANALOG_MIN)
	{
		position.y += _camera.GetMoveCameraSpeed();
	}

	//カメラの高さの上限を設定
	float camera_max_y = MV1GetPosition(resource::ResourceServer::GetModelHandle("player")).y + 400.0f;
	if (position.y > camera_max_y)
		position.y = camera_max_y;

	//カメラの高さを下に移動
	if (stick_ry > ANALOG_MIN)
	{
		position.y -= _camera.GetMoveCameraSpeed();
	}

	//カメラの高さの下限を設定
	float camera_y_min = 80;
	if (position.y < camera_y_min)
		position.y = camera_y_min;

	//カメラと床の押し出し処理
	while (1)
	{
		//プレイヤーのカプセル情報
		VECTOR sphere_position = position;
		float radius = 60.0f;

		//床との当たり判定をとる
		MV1_COLL_RESULT_POLY_DIM hit_poly_floorpush;
		hit_poly_floorpush = stage::Stage::GetInstance()->GetHitSphereToFloor(sphere_position, radius);

		//判定がなければ終了
		if (hit_poly_floorpush.HitNum == 0)
		{
			MV1CollResultPolyDimTerminate(hit_poly_floorpush);
			break;
		}

		//法線ベクトルを算出
		VECTOR normal = { 0,0,0 };
		for (int i = 0; i < hit_poly_floorpush.HitNum; i++)
		{
			normal = VAdd(normal, VNorm(hit_poly_floorpush.Dim[i].Normal));
		}
		normal = VNorm(normal);

		//押し出し
		position = VAdd(position, VScale(normal, 0.5f));

		MV1CollResultPolyDimTerminate(hit_poly_floorpush);
	}

	_camera.SetPosition(position);
}