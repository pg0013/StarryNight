/**
 * @file    ModeGameClear.h
 * @brief  ゲームクリアシーン
 *
 * @author Takuya Fujisawa
 * @date   2021/03/05
 */

#include "ModeGameClear.h"
#include"ModeOverlay.h"
#include"ModeGame.h"
#include"ModeTitle.h"

using namespace starrynight::mode;

ModeGameClear::ModeGameClear(int _score)
{
	score_ui_.SetPlayerScore(_score);
	score_ui_.SetPosition(VGet(1100, 640, 0));
	score_ui_.SetDrawScoreBaseFlag(false);

	//画像パラメータを読み込み
	param_.LoadImagParameters("gameclear");

	background_graph_ = resource::ResourceServer::GetTexture("background_gameclear.png");
	gameclear_graph_ = resource::ResourceServer::GetTexture("gameclear.png");
	next_graph_ = resource::ResourceServer::GetTexture("next.png");
	next_base_graph_ = resource::ResourceServer::GetTexture("cursol.png");
	return_graph_ = resource::ResourceServer::GetTexture("return.png");
	return_base_graph_ = resource::ResourceServer::GetTexture("cursol.png");

	cursol_ = NEXT_GAME;
	menu_num_ = 2;
	nextmode_count_ = 0;
	pushed_flag_ = false;
}

ModeGameClear::~ModeGameClear()
{
}

bool ModeGameClear::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	ModeOverlay* modeoverlay = NEW ModeOverlay();
	modeoverlay->Fade(30, FADE_IN);
	::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

	score_ui_.Initialize();

	appframe::ApplicationBase::GetInstance()->bgm_.Load("Resource/sound/gameclear.wav");

	appframe::ApplicationBase::GetInstance()->bgm_.SetVolume(1.0f);
	appframe::ApplicationBase::GetInstance()->bgm_.SetLoopCount(0);
	appframe::ApplicationBase::GetInstance()->bgm_.Play();

	return true;
}

bool ModeGameClear::Terminate()
{
	::mode::ModeBase::Terminate();

	score_ui_.Terminate();

	appframe::ApplicationBase::GetInstance()->bgm_.Pause();
	appframe::ApplicationBase::GetInstance()->bgm_.Destroy();

	return true;
}

bool ModeGameClear::Process()
{
	::mode::ModeBase::Process();

	Input();
	NextMode();

	score_ui_.Process();

	return true;
}

void ModeGameClear::Input()
{
	if (pushed_flag_ == true)
		return;

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	if (trigger_key & PAD_INPUT_LEFT) { cursol_--; }
	if (trigger_key & PAD_INPUT_RIGHT) { cursol_++; }

	cursol_ = (cursol_ + menu_num_) % menu_num_;

	if (trigger_key & PAD_INPUT_1)
	{
		pushed_flag_ = true;

		nextmode_count_ = 60;

		ModeOverlay* modeoverlay = NEW ModeOverlay();
		modeoverlay->Fade(nextmode_count_, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

		appframe::ApplicationBase::GetInstance()->se_.SetVolume(1.0f);
		appframe::ApplicationBase::GetInstance()->se_.Play();
		appframe::ApplicationBase::GetInstance()->se_.Fade(0.0f, 1.0f);
		appframe::ApplicationBase::GetInstance()->bgm_.Fade(0.0f, 1.0f);
	}
}

void ModeGameClear::NextMode()
{
	if (pushed_flag_ == false)
		return;

	nextmode_count_--;

	if (nextmode_count_ != 0)
		return;

	if (cursol_ == NEXT_GAME)
	{
		resource::ResourceServer::ClearModelMap();

		ModeGame* mode_game = NEW ModeGame("haru_A");
		::mode::ModeServer::GetInstance()->Add(mode_game, 0, "Game");
		::mode::ModeServer::GetInstance()->Del(this);
	}

	if (cursol_ == RETURN_TITLE)
	{
		ModeTitle* mode_title = NEW ModeTitle();
		::mode::ModeServer::GetInstance()->Add(mode_title, 0, "Title");
		::mode::ModeServer::GetInstance()->Del(this);
	}
}

bool ModeGameClear::Render()
{
	::mode::ModeBase::Render();

	DrawGraph(0, 0, background_graph_, TRUE);
	DrawGraph(0, 0, gameclear_graph_, TRUE);

	//戻る
	DrawRotaGraph(1380, 850, 1.0, 0.0, return_graph_, TRUE);

	if (cursol_ == RETURN_TITLE)
	{
		//ボタンが押されたら点滅速度を上げる
		if (pushed_flag_ == false)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 2.125f * GetModeCount()))));
		else
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 10 * GetModeCount()))));
		DrawRotaGraph(1380, 880, 1.0, 0.0, return_base_graph_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//タイトルへ戻る
	DrawRotaGraph(525, 850, 1.0, 0.0, next_graph_, TRUE);

	if (cursol_ == NEXT_GAME)
	{
		//ボタンが押されたら点滅速度を上げる
		if (pushed_flag_ == false)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 2.125f * GetModeCount()))));
		else
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 10 * GetModeCount()))));
		DrawRotaGraph(550, 880, 1.0, 0.0, next_base_graph_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	score_ui_.Render();

	return true;
}