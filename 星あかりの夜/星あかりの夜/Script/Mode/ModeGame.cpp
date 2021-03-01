/**
 * @file    ModeGame.cpp
 * @brief  ÉQÅ[ÉÄÉÇÅ[ÉhÇÃèàóù
 *
 * @author Takuya Fujisawa
 * @date    202012/09
 */
#include "ModeGame.h"
#include"../ApplicationMain.h"
#include"../Player/Player.h"
using namespace starrynight::mode;

ModeGame::ModeGame()
{
	stop_object_process_ = false;
	player_star_num_ = 0;
	game_score_ = 0;
	regulations_score_ = 30000;
}

ModeGame::~ModeGame()
{
}

bool ModeGame::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	object::ObjectBase* player = NEW player::Player();
	object_server_.Add(player);

	camera_.Initialize();
	stage_.Initialize();
	ui_.Initialize();

	return true;
}

bool ModeGame::Terminate()
{
	::mode::ModeBase::Terminate();

	object_server_.Clear();
	effect_server_.Clear();
	ui_.Terminate();

	return true;
}

bool ModeGame::Process()
{
	::mode::ModeBase::Process();

	if (!stop_object_process_)
	{
		object_server_.Process();
		effect_server_.Process();
	}

	camera_.Process();
	ui_.Process();

	return true;
}

bool ModeGame::Render()
{
	::mode::ModeBase::Render();

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	SetLightAmbColor(GetColorF(0.2f, 0.2f, 0.2f, 0.0f));

	camera_.Render();

	stage_.Render();
	object_server_.Render();
	effect_server_.Render();
	ui_.Render();

#ifdef DEBUG_FUNCTION
	int x, y;
	camera_.DrawDebugMenu();
	utility::DrawModelDebugInfo(resource::ResourceServer::GetModelHandle("player"), "Player", 7);
	utility::Draw3DAxis(10.f, camera_.GetTarget());
	x = 0; y = 11 * DEBUG_FONT_SIZE;
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- star    : %d", y / DEBUG_FONT_SIZE, GetPlayerStarNum()); y += DEBUG_FONT_SIZE;
	y = 13 * DEBUG_FONT_SIZE;
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- RegScore    : %d", y / DEBUG_FONT_SIZE, GetStageRegulationsScore()); y += DEBUG_FONT_SIZE;
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- GameScore   : %d", y / DEBUG_FONT_SIZE,GetGameScore()); y += DEBUG_FONT_SIZE;
#endif

	return true;
}