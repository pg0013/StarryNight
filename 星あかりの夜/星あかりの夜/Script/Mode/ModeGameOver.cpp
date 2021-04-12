/**
 * @file    ModeGameOver.cpp
 * @brief  ゲームオーバーシーン
 *
 * @author Takuya Fujisawa
 * @date   2021/03/08
 */

#include "ModeGameOver.h"
#include"ModeOverlay.h"
#include"ModeGame.h"
#include"ModeLoading.h"
#include"ModeTitle.h"

using namespace starrynight::mode;

ModeGameOver::ModeGameOver(const std::string& _stage_name)
{
	//画像パラメータを読み込み
	param_.LoadImagParameters("gameover");

	background_graph_ = resource::ResourceServer::GetTexture("background_gameover.png");
	again_graph_ = resource::ResourceServer::GetTexture("again.png");
	return_graph_ = resource::ResourceServer::GetTexture("return_gameover.png");

	cursol_ = AGAIN;
	menu_num_ = 2;
	nextmode_count_ = 0;
	pushed_flag_ = false;

	stage_name_ = _stage_name;
}

ModeGameOver::~ModeGameOver()
{
}

bool ModeGameOver::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	ModeOverlay* modeoverlay = NEW ModeOverlay();
	modeoverlay->Fade(30, FADE_IN);
	::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

	appframe::ApplicationBase::GetInstance()->bgm_.Load("Resource/sound/gameover.wav");

	appframe::ApplicationBase::GetInstance()->bgm_.SetVolume(1.0f);
	appframe::ApplicationBase::GetInstance()->bgm_.SetLoopCount(0);
	appframe::ApplicationBase::GetInstance()->bgm_.Play();

	return true;
}

bool ModeGameOver::Terminate()
{
	::mode::ModeBase::Terminate();

	appframe::ApplicationBase::GetInstance()->bgm_.Pause();
	appframe::ApplicationBase::GetInstance()->bgm_.Destroy();

	return true;
}

bool ModeGameOver::Process()
{
	::mode::ModeBase::Process();

	Input();
	NextMode();

	return true;
}

void ModeGameOver::Input()
{
	if (pushed_flag_ == true)
		return;

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	if (trigger_key & PAD_INPUT_LEFT)
	{
		cursol_--;
		appframe::ApplicationBase::GetInstance()->se_.Load("Resource/sound/se2.wav");
		appframe::ApplicationBase::GetInstance()->se_.Play();
	}
	if (trigger_key & PAD_INPUT_RIGHT)
	{
		cursol_++;
		appframe::ApplicationBase::GetInstance()->se_.Load("Resource/sound/se2.wav");
		appframe::ApplicationBase::GetInstance()->se_.Play();
	}

	cursol_ = (cursol_ + menu_num_) % menu_num_;

	if (trigger_key & PAD_INPUT_2)
	{
		pushed_flag_ = true;

		nextmode_count_ = 60;

		ModeOverlay* modeoverlay = NEW ModeOverlay();
		modeoverlay->Fade(nextmode_count_, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

		if (cursol_ == AGAIN)
		{
			ModeLoading* mode_loading = NEW ModeLoading();
			::mode::ModeServer::GetInstance()->Add(mode_loading, 10, "Loading");
		}
		else
		{
			appframe::ApplicationBase::GetInstance()->se_.Load("Resource/sound/se2.wav");
			appframe::ApplicationBase::GetInstance()->se_.SetVolume(1.0f);
			appframe::ApplicationBase::GetInstance()->se_.Play();
			appframe::ApplicationBase::GetInstance()->se_.Fade(0.0f, 0.8f);

			appframe::ApplicationBase::GetInstance()->bgm_.Fade(0.0f, 0.8f);
		}
	}
}

void ModeGameOver::NextMode()
{
	if (pushed_flag_ == false)
		return;

	nextmode_count_--;

	if (nextmode_count_ != 0)
		return;

	if (cursol_ == AGAIN)
	{
		resource::ResourceServer::ClearModelMap();

		ModeGame* mode_game = NEW ModeGame(stage_name_);
		::mode::ModeServer::GetInstance()->Add(mode_game, 0, "Game");
		::mode::ModeServer::GetInstance()->Del(this);
	}

	if (cursol_ == GIVEUP)
	{
		ModeTitle* mode_title = NEW ModeTitle();
		::mode::ModeServer::GetInstance()->Add(mode_title, 0, "Title");
		::mode::ModeServer::GetInstance()->Del(this);
	}
}

bool ModeGameOver::Render()
{
	::mode::ModeBase::Render();

	DrawGraph(0, 0, background_graph_, TRUE);

	//戻る

	if (cursol_ == GIVEUP)
	{
		//選択されたら点滅する
		if (pushed_flag_ == false)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 2.125f * GetModeCount()))));

		DrawRotaGraph(1380, 850, 1.0, 0.0, return_graph_, TRUE);
	}
	else
	{
		DrawRotaGraph(1380, 850, 1.0, 0.0, return_graph_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//タイトルへ戻る
	if (cursol_ == AGAIN)
	{
		//選択されたら点滅する
		if (pushed_flag_ == false)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 2.125f * GetModeCount()))));

		DrawRotaGraph(525, 850, 1.0, 0.0, again_graph_, TRUE);
	}
	else
	{
		DrawRotaGraph(525, 850, 1.0, 0.0, again_graph_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	return true;
}