/**
 * @file    SkyStarCamera.cpp
 * @brief  星座完成エフェクトを見せるカメラ
 *
 * @author Takuya Fujisawa
 * @date   2021/03/05
 */

#include "Camera.h"
#include"../Mode/ModeGame.h"
#include"../Mode/ModeOverlay.h"
using namespace starrynight::camera;

void Camera::SkyStarCameraInit()
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	for (auto iter = mode_game->object_server_.List()->begin(); iter != mode_game->object_server_.List()->end(); iter++)
	{
		if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::SKY_STAR)
		{
			target_ = (*iter)->GetPosition();
		}
	}

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));

	position_ = VAdd(player_position, VScale(VSub(target_, player_position), 0.1f));

	mode_game->ui_.SetDrawUIFlag(false);

	int fade_count = 90;
	mode::ModeOverlay* modeoverlay = NEW mode::ModeOverlay();
	modeoverlay->Fade(fade_count, FADE_IN);
	::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

	appframe::ApplicationBase::GetInstance()->bgm_.Fade(0.0f, 1.5f);
}