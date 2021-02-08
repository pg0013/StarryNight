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
}

ModeGame::~ModeGame()
{
}

bool ModeGame::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	camera_.Initialize();
	stage_.Initialize();
	ui_.Initialize();

	object::ObjectBase* player = NEW player::Player();
	object_server_.Add(player);

	return true;
}

bool ModeGame::Terminate()
{
	::mode::ModeBase::Terminate();

	object_server_.Clear();
	ui_.Terminate();

	return true;
}

bool ModeGame::Process()
{
	::mode::ModeBase::Process();

	if (!stop_object_process_)
	{
		object_server_.Process();
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
	SetUseBackCulling(TRUE);

	camera_.Render();

	object_server_.Render();
	stage_.Render();
	ui_.Render();
	
#ifdef DEBUG_FUNCTION
	camera_.DrawDebugMenu();
	utility::DrawModelDebugInfo(resource::ResourceServer::GetModelHandle("player"), "Player", 7);
	utility::Draw3DAxis(10.f, camera_.GetTarget());
#endif

	return true;
}