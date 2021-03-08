/**
 * @file    ApplicationMain.cpp
 * @brief  このプロジェクトのメイン処理
 *
 * @author Takuya Fujisawa
 * @date    20201021
 */

#include "ApplicationMain.h"
#include"Mode/ModeTitle.h"

using namespace starrynight;

ApplicationMain application_main;

bool ApplicationMain::Initialize(HINSTANCE _hinstance)
{
	if (!appbase::Initialize(_hinstance)) { return false; }

	mode::ModeTitle* mode_title = NEW mode::ModeTitle();
	::mode::ModeServer::GetInstance()->Add(mode_title, 0, "Title");

	resource::ResourceServer::LoadSound("Resource/sound/outgame_bgm.wav");
	resource::ResourceServer::LoadSound("Resource/sound/se1.wav");

	bgm_.Load("Resource/sound/outgame_bgm.wav");
	se_.Load("Resource/sound/se1.wav");

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