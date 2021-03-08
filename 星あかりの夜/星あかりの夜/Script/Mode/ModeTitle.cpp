/**
 * @file    ModeTitle.cpp
 * @brief
 *
 * @author Takuya Fujisawa
 * @date   2021/03/03
 */

#include "ModeTitle.h"
#include"ModeOverlay.h"
#include"ModeMenu.h"

using namespace starrynight::mode;

ModeTitle::ModeTitle()
{
	//画像パラメータを読み込み
	param_.LoadImagParameters("title");

	//画像ハンドルを記録
	background_graph_ = resource::ResourceServer::GetTexture("title_background.png");
	start_graph_ = resource::ResourceServer::GetTexture("start.png");
	startline_graph_ = resource::ResourceServer::GetTexture("start_line.png");
	charactor_graph_ = resource::ResourceServer::GetTexture("charactor.png");

	pushed_flag_ = false;
	nextmode_count_ = 0;
	nextmode_ = MENU;
}

ModeTitle::~ModeTitle()
{
}

bool ModeTitle::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	ModeOverlay* modeoverlay = NEW ModeOverlay();
	modeoverlay->Fade(60, FADE_IN);
	::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

	if (appframe::ApplicationBase::GetInstance()->bgm_.CheckIsRunning() == false)
	{
		appframe::ApplicationBase::GetInstance()->bgm_.Load("Resource/sound/outgame_bgm.wav");
		appframe::ApplicationBase::GetInstance()->bgm_.SetVolume(1.0f);
		appframe::ApplicationBase::GetInstance()->bgm_.SetLoopCount(XAUDIO2_LOOP_INFINITE);
		appframe::ApplicationBase::GetInstance()->bgm_.PlayWithLoop(0.0f, 125.0f);
	}

	return true;
}

bool ModeTitle::Terminate()
{
	::mode::ModeBase::Terminate();

	appframe::ApplicationBase::GetInstance()->se_.Fade(0.0f, 0.1f);
	return true;
}

bool ModeTitle::Process()
{
	::mode::ModeBase::Process();

	Input();
	NextMode();
	return true;
}

bool ModeTitle::Render()
{
	::mode::ModeBase::Render();
	DrawGraph(0, 0, background_graph_, TRUE);
	DrawGraph(0, 0, startline_graph_, TRUE);
	DrawGraph(0, 0, charactor_graph_, TRUE);

	//ボタンが押されたら点滅速度を上げる
	if (pushed_flag_ == false)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 2.125f * GetModeCount()))));
	else
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 10 * GetModeCount()))));

	DrawGraph(0, 0, start_graph_, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	return true;
}

void ModeTitle::Input()
{
	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	if (pushed_flag_ == true)
		return;

	if (trigger_key & PAD_INPUT_1)
	{
		pushed_flag_ = true;

		nextmode_count_ = 60;
		nextmode_ = MENU;

		ModeOverlay* modeoverlay = NEW ModeOverlay();
		modeoverlay->Fade(nextmode_count_, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

		appframe::ApplicationBase::GetInstance()->se_.Play();
	}
	if (trigger_key & PAD_INPUT_2)
	{
		pushed_flag_ = true;

		nextmode_count_ = 60;
		nextmode_ = EXIT;

		ModeOverlay* modeoverlay = NEW ModeOverlay();
		modeoverlay->Fade(nextmode_count_, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

		appframe::ApplicationBase::GetInstance()->se_.Play();
		appframe::ApplicationBase::GetInstance()->se_.Fade(0.0f, 1.0f);
		appframe::ApplicationBase::GetInstance()->bgm_.Fade(0.0f, 1.0f);
	}
}

void ModeTitle::NextMode()
{
	if (pushed_flag_ == false)
		return;

	nextmode_count_--;

	if (nextmode_count_ > 0)
		return;

	if (nextmode_ == MENU)
	{
		ModeMenu* mode_menu = NEW ModeMenu();
		::mode::ModeServer::GetInstance()->Add(mode_menu, 0, "Menu");
		::mode::ModeServer::GetInstance()->Del(this);
	}
	else if (nextmode_ == EXIT)
	{
		::mode::ModeServer::GetInstance()->Del(this);
		appframe::ApplicationBase::GetInstance()->ExitProgram();
	}
}