/**
 * @file    CameraSkyStarState.cpp
 * @brief  星座完成時のカメラ処理
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include "CameraSkyStarState.h"
#include"Camera.h"
#include"../Player/Player.h"
#include"../Mode/ModeGame.h"
#include"../Mode/ModeOverlay.h"

using namespace starrynight::camera;

CameraSkyStarState::CameraSkyStarState()
{
}

CameraSkyStarState::~CameraSkyStarState()
{
}

void CameraSkyStarState::Enter(Camera& _camera)
{
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	VECTOR position = _camera.GetPosition();
	VECTOR target = _camera.GetTarget();

	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::SKY_STAR)
		{
			//ターゲットを星座に設定
			target = (iter)->GetPosition();
		}
	}

	//星座がよく見える位置にカメラを移動
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	position = VAdd(player_position, VScale(VSub(target, player_position), 0.1f));

	//UIを非表示
	mode_game->ui_.SetDrawUIFlag(false);

	int fade_count = 90;
	std::shared_ptr<mode::ModeOverlay> modeoverlay = std::make_shared < mode::ModeOverlay >();
	modeoverlay->Fade(fade_count, FADE_IN);
	::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

	appframe::ApplicationBase::GetInstance()->bgm_.Fade(0.0f, 1.5f);

	//座標とターゲットを設定
	_camera.SetPosition(position);
	_camera.SetTarget(target);
}

void CameraSkyStarState::Exit(Camera& _camera)
{
}

void CameraSkyStarState::Input(Camera& _camera)
{
}

void CameraSkyStarState::Update(Camera& _camera)
{
}