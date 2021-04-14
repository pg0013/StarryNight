/**
 * @file    CameraShootState.cpp
 * @brief  カメラの射撃状態処理
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include "CameraShootState.h"
#include"Camera.h"
#include"../Player/Player.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::camera;

CameraShootState::CameraShootState()
{
	old_target_ = VGet(0, 5000, 0);
}

CameraShootState::~CameraShootState()
{
}

void CameraShootState::Enter(Camera& _camera)
{
	VECTOR target = _camera.GetTarget();

	//プレイヤーの情報を取得
	VECTOR player_positon = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	player_positon = VAdd(player_positon, VGet(0.0f, 85.0f, 0.0f));
	VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));
}

void CameraShootState::Exit(Camera& _camera)
{
}

void CameraShootState::Input(Camera& _camera)
{
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();

	bool slingshot_flag = true;
	float anim_playtime = 0.0f;
	float shoot_anim_end = 80.0f;

	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if (iter->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(iter);
			//プレイヤーが射撃状態か取得
			slingshot_flag = player->GetPlayerSlingShotStatus();
			//アニメーション再生時間を取得
			anim_playtime = player->GetAnimPlayTime();
		}
	}

	//射撃状態でステージクリアすれば、星座完成カメラへ移行
	if (mode_game->IsClearStage() == true &&
		anim_playtime > shoot_anim_end)
	{
		_camera.SetStatus(Camera::STATUS::SKYSTAR);
		_camera.ChangeCameraState("SkyStar");
		return;
	}

	//プレイヤーが射撃を解除すれば移動カメラに移行
	if (slingshot_flag == false)
	{
		_camera.SetStatus(Camera::STATUS::MOVE);
		_camera.ChangeCameraState("Move");
		return;
	}
}

void CameraShootState::Update(Camera& _camera)
{
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();

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

	SetShootCameraPosition(_camera);
	SetShootCameraTarget(_camera);
}