/**
 * @file    ModeGame.cpp
 * @brief  ƒQ[ƒ€ƒ‚[ƒh‚Ìˆ—
 *
 * @author Takuya Fujisawa
 * @date    202012/09
 */

#include "ModeGame.h"
#include"ModeGameClear.h"
#include"ModeOverlay.h"
#include"../Player/Player.h"

using namespace starrynight::mode;

ModeGame::ModeGame()
{
	stop_object_process_ = false;
	player_star_num_ = 7;
	game_score_ = 0;
	regulations_score_ = 30000;
	fade_count_ = 0;
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

	NextMode();
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
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- GameScore   : %d", y / DEBUG_FONT_SIZE, GetGameScore()); y += DEBUG_FONT_SIZE;
#endif

	return true;
}

void ModeGame::SetNextMode(int _count, int _fade_count)
{
	pushed_flag_ = true;

	nextmode_count_ = _count;
	fade_count_ = _fade_count;
}

void ModeGame::NextMode()
{
	if (pushed_flag_ == false)
		return;

	nextmode_count_--;

	if (nextmode_count_ == fade_count_)
	{
		ModeOverlay* modeoverlay = NEW ModeOverlay();
		modeoverlay->Fade(fade_count_, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");
	}

	if (nextmode_count_ == 0)
	{
		ModeGameClear* mode_gameclear = NEW ModeGameClear(game_score_);
		::mode::ModeServer::GetInstance()->Add(mode_gameclear, 0, "GameClear");
		::mode::ModeServer::GetInstance()->Del(this);
	}
}