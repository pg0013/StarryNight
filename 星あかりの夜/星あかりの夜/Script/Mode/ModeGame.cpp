/**
 * @file    ModeGame.cpp
 * @brief  ÉQÅ[ÉÄÉÇÅ[ÉhÇÃèàóù
 * 
 * @author Takuya Fujisawa
 * @date    202012/09
 */
#include "ModeGame.h"
#include"../ApplicationMain.h"

using namespace starrynight;

ModeGame::ModeGame()
{
	camera_.position_ = VGet(73.636536f, 86.688026f, -140.440582f);
	camera_.target_ = VGet(-12.230986f, 59.101776f, -15.002045f);
	camera_.clip_near_ = 2.376863f;
	camera_.clip_far_ = 594.215820f;

	resource::ResourceServer::RegisterModel("Game", "human", false);
	player_ = resource::ResourceServer::GetModelHandle("human");
	resource::ResourceServer::RegisterModel("Game", "field_ver1", false);
	stage_ = resource::ResourceServer::GetModelHandle("field_ver1");
}

ModeGame::~ModeGame()
{
}

bool ModeGame::Initialize()
{

	return true;
}

bool ModeGame::Terminate()
{
	return true;
}

bool ModeGame::Process()
{

	return true;
}

bool ModeGame::Render()
{
	SetCameraPositionAndTarget_UpVecY(camera_.position_,camera_.target_);
	SetCameraNearFar(camera_.clip_near_,camera_.clip_far_);

	MV1DrawModel(player_);
	MV1DrawModel(stage_);

	utility::Draw3DAxis(10.f, camera_.target_);

	return true;
}