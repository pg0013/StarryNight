/**
 * @file    ApplicationMain.cpp
 * @brief  このプロジェクトのメイン処理
 *
 * @author Takuya Fujisawa
 * @date    20201021
 */

#include "ApplicationMain.h"
#include"Mode/ModeGame.h"
#include"Mode/ModeTitle.h"
#include"Mode/ModeGameClear.h"

using namespace starrynight;

ApplicationMain application_main;

bool ApplicationMain::Initialize(HINSTANCE _hinstance)
{
	if (!appbase::Initialize(_hinstance)) { return false; }

	//mode::ModeTitle* mode_title = NEW mode::ModeTitle();
	//::mode::ModeServer::GetInstance()->Add(mode_title, 0, "Title");

	mode::ModeGame* mode_game = NEW mode::ModeGame();
	::mode::ModeServer::GetInstance()->Add(mode_game, 0, "Game");

	//mode::ModeGameClear* mode_gameclear = NEW mode::ModeGameClear(35000);
	//::mode::ModeServer::GetInstance()->Add(mode_gameclear, 0, "GameClear");

	return true;
}

bool ApplicationMain::Terminate()
{
	appbase::Terminate();
	return true;
}

bool ApplicationMain::Input()
{
	appbase::Input();
	return true;
}

bool ApplicationMain::Process()
{
	appbase::Process();

	return true;
}

bool ApplicationMain::Render()
{
	appbase::Render();

	return true;
}