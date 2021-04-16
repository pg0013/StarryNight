/**
 * @file    CameraMoveState.cpp
 * @brief  移動カメラの状態処理
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include "CameraMoveState.h"
#include"Camera.h"
#include"../Player/Player.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::camera;

CameraMoveState::CameraMoveState()
{
}

CameraMoveState::~CameraMoveState()
{
}

void CameraMoveState::Enter(Camera& _camera)
{
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));

	VECTOR position = VAdd(VAdd(player_position, VGet(0, 90, 0)),
		VScale(utility::GetForwardVector(player_rotation.y), _camera.GetCameraDistance()));

	_camera.SetTarget(VAdd(VGet(0.0f, 60.0f, 0.0f), player_position));
	_camera.SetPosition(position);
}

void CameraMoveState::Exit(Camera& _camera)
{
}

void CameraMoveState::Input(Camera& _camera)
{
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	bool slingshot_flag = false;
	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if (iter->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			std::shared_ptr<player::Player> player = std::dynamic_pointer_cast<player::Player>(iter);
			//プレイヤーが射撃状態か取得
			slingshot_flag = player->GetPlayerSlingShotStatus();
		}
	}

	//プレイヤーが射撃状態であれば射撃カメラに移行
	if (slingshot_flag == true)
	{
		_camera.SetStatus(Camera::STATUS::SHOOT);
		_camera.ChangeCameraState("Shoot");
	}
}

void CameraMoveState::Update(Camera& _camera)
{
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	//ゲームオーバーであれば、プレイヤーの処理を行わない
	if (mode_game->GetIsGameOver() == true)
		return;

	MoveCamera(_camera);
}