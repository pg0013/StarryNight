/**
 * @file    ModePauseMenu.cpp
 * @brief  ゲーム中のポーズメニューシーン
 *
 * @author Takuya Fujisawa
 * @date   2021/03/06
 */

#include "ModePauseMenu.h"
#include"ModeOverlay.h"
#include"ModeTitle.h"
#include"ModeTutorial.h"
#include"ModeGame.h"

using namespace starrynight::mode;

ModePauseMenu::ModePauseMenu()
{
	//画像パラメータを読み込み
	param_.LoadImagParameters("pausemenu");

	background_graph_ = resource::ResourceServer::GetTexture("background_pausemenu.png");
	overlay_graph_ = resource::ResourceServer::GetTexture("overlay.png");
	pause_graph_ = resource::ResourceServer::GetTexture("pause.png");
	return_graph_[0] = resource::ResourceServer::GetTexture("return.png");
	return_graph_[1] = resource::ResourceServer::GetTexture("return_base.png");
	return_title_graph_[0] = resource::ResourceServer::GetTexture("return_title.png");
	return_title_graph_[1] = resource::ResourceServer::GetTexture("return_title_base.png");
	tutorial_graph_[0] = resource::ResourceServer::GetTexture("tutorial.png");
	tutorial_graph_[1] = resource::ResourceServer::GetTexture("tutorial_base.png");

	cursol_ = 0;
	menu_num_ = 3;
	nextmode_count_ = 0;
	nextmode_ = TUTORIAL;
	pushed_flag_ = false;
}

ModePauseMenu::~ModePauseMenu()
{
}

bool ModePauseMenu::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	ModeOverlay* modeoverlay = NEW ModeOverlay();
	modeoverlay->Capture(20);
	::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

	return true;
}

bool ModePauseMenu::Terminate()
{
	::mode::ModeBase::Terminate();

	return true;
}

bool ModePauseMenu::Process()
{
	::mode::ModeBase::Process();

	//下のレイヤーにあるゲームシーンの処理を止める
	::mode::ModeServer::GetInstance()->SkipProcessUnderLayer();

	Input();
	NextMode();

	return true;
}

void ModePauseMenu::Input()
{
	if (pushed_flag_ == true)
		return;

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	if (trigger_key & PAD_INPUT_UP) { cursol_--; }
	if (trigger_key & PAD_INPUT_DOWN) { cursol_++; }

	cursol_ = (cursol_ + menu_num_) % menu_num_;
	nextmode_ = cursol_;

	//Aボタンでゲームに進む
	if (trigger_key & PAD_INPUT_1)
	{
		pushed_flag_ = true;

		nextmode_count_ = 60;
		nextmode_ = cursol_;

		if (nextmode_ == TITLE)
		{
			ModeOverlay* modeoverlay = NEW ModeOverlay();
			modeoverlay->Fade(nextmode_count_, FADE_OUT);
			::mode::ModeServer::GetInstance()->Add(modeoverlay, 2, "Overlay");

			appframe::ApplicationBase::GetInstance()->bgm_.Fade(0.0f, 1.0f);
		}
		else if (nextmode_ == RETURN)
		{
			nextmode_count_ = 1;
		}
		else if (nextmode_ == TUTORIAL)
		{
			nextmode_count_ = 2;

			ModeTutorial* mode_tutorial = NEW ModeTutorial();
			::mode::ModeServer::GetInstance()->Add(mode_tutorial, 3, "Tutorial");
		}
	}

	//STARTボタンでポーズメニューを表示
	if (trigger_key & PAD_INPUT_8)
	{
		pushed_flag_ = true;
		nextmode_ = RETURN;

		nextmode_count_ = 1;
	}
}

void ModePauseMenu::NextMode()
{
	if (pushed_flag_ == false)
		return;

	nextmode_count_--;

	if (nextmode_count_ > 0)
		return;

	if (nextmode_ == TITLE)
	{
		resource::ResourceServer::ClearModelMap();

		ModeTitle* mode_title = NEW ModeTitle();
		::mode::ModeServer::GetInstance()->Add(mode_title, 0, "Title");
		::mode::ModeServer::GetInstance()->Del(::mode::ModeServer::GetInstance()->Get("Game"));
		::mode::ModeServer::GetInstance()->Del(this);
	}
	else if (nextmode_ == RETURN)
	{
		mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
		mode_game->ui_.SetDrawUIFlag(true);
		::mode::ModeServer::GetInstance()->Del(this);
	}
	else if (nextmode_ == TUTORIAL)
	{
		pushed_flag_ = false;
	}
}

bool ModePauseMenu::Render()
{
	::mode::ModeBase::Render();

	DrawGraph(0, 0, overlay_graph_, TRUE);
	DrawGraph(0, 0, background_graph_, TRUE);
	DrawGraph(0, 0, pause_graph_, TRUE);

	DrawGraph(0, 0, tutorial_graph_[1], TRUE);

	if (cursol_ == TUTORIAL)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 10 * GetModeCount()))));
	DrawGraph(0, 0, tutorial_graph_[0], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawGraph(0, 0, return_graph_[1], TRUE);

	if (cursol_ == RETURN)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 10 * GetModeCount()))));
	DrawGraph(0, 0, return_graph_[0], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawGraph(0, 0, return_title_graph_[1], TRUE);

	if (cursol_ == TITLE)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 10 * GetModeCount()))));
	DrawGraph(0, 0, return_title_graph_[0], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	return true;
}