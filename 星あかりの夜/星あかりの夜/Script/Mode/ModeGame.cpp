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
	game_param_.LoadModel("Game", false);

	camera_.Initialize();
	stage_.Initialize();
	player_ = resource::ResourceServer::GetModelHandle("human_GEO");
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
	camera_.Process();

	return true;
}

bool ModeGame::Render()
{
	camera_.Render();
	MV1DrawModel(player_);
	stage_.Render();

#ifdef DEBUG_FUNCTION
	camera_.DrawDebugMenu();
	utility::Draw3DAxis(10.f, camera_.GetTarget());
#endif

	return true;
}