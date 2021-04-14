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
}

void CameraMoveState::Exit(Camera& _camera)
{
}

void CameraMoveState::Input(Camera& _camera)
{
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();

	bool slingshot_flag = false;
	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if (iter->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(iter);
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
	MoveCamera(_camera);
}