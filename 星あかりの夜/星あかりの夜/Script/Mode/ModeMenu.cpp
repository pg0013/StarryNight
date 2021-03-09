/**
 * @file    ModeMenu.h
 * @brief  メニュー選択シーン
 *
 * @author Takuya Fujisawa
 * @date   2021/03/06
 */

#include "ModeMenu.h"
#include"ModeOverlay.h"
#include"ModeGame.h"
#include"ModeTitle.h"

using namespace starrynight::mode;

ModeMenu::ModeMenu()
{
	//画像パラメータを読み込み
	param_.LoadImagParameters("menu");

	background_graph_ = resource::ResourceServer::GetTexture("background.png");
	stage_num_graph_[0] = resource::ResourceServer::GetTexture("1.png");
	stage_num_graph_[1] = resource::ResourceServer::GetTexture("2.png");
	stage_num_graph_[2] = resource::ResourceServer::GetTexture("3.png");
	stage_graph_[0] = resource::ResourceServer::GetTexture("haru_A.png");
	stage_graph_[1] = resource::ResourceServer::GetTexture("haru_B.png");
	stage_graph_[2] = resource::ResourceServer::GetTexture("haru_C.png");
	spring_icon_ = resource::ResourceServer::GetTexture("spring_icon.png");
	cloud_graph_ = resource::ResourceServer::GetTexture("cloud.png");
	menu_graph_ = resource::ResourceServer::GetTexture("menu.png");
	stage_goal_graph_ = resource::ResourceServer::GetTexture("stage_goal.png");

	cursol_ = 0;
	menu_num_ = 3;
	nextmode_count_ = 0;
	nextmode_ = HARU_A;
	pushed_flag_ = false;
}

ModeMenu::~ModeMenu()
{
}

bool ModeMenu::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	ModeOverlay* modeoverlay = NEW ModeOverlay();
	modeoverlay->Fade(30, FADE_IN);
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

bool ModeMenu::Terminate()
{
	::mode::ModeBase::Terminate();

	return true;
}

bool ModeMenu::Process()
{
	::mode::ModeBase::Process();

	Input();
	NextMode();

	return true;
}

void ModeMenu::Input()
{
	if (pushed_flag_ == true)
		return;

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	if (trigger_key & PAD_INPUT_LEFT) { cursol_--; }
	if (trigger_key & PAD_INPUT_RIGHT) { cursol_++; }

	cursol_ = (cursol_ + menu_num_) % menu_num_;
	nextmode_ = cursol_;

	//Bボタンでゲームに進む
	if (trigger_key & PAD_INPUT_2)
	{
		pushed_flag_ = true;

		nextmode_count_ = 60;
		nextmode_ = cursol_;

		ModeOverlay* modeoverlay = NEW ModeOverlay();
		modeoverlay->Fade(nextmode_count_, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

		appframe::ApplicationBase::GetInstance()->se_.Load("Resource/sound/se1.wav");
		appframe::ApplicationBase::GetInstance()->se_.SetVolume(1.0f);
		appframe::ApplicationBase::GetInstance()->se_.Play();
		appframe::ApplicationBase::GetInstance()->se_.Fade(0.0f, 1.0f);
		appframe::ApplicationBase::GetInstance()->bgm_.Fade(0.0f, 1.0f);
	}

	//Aボタンでタイトルに戻る
	if (trigger_key & PAD_INPUT_1)
	{
		pushed_flag_ = true;

		nextmode_count_ = 60;
		nextmode_ = TITLE;

		ModeOverlay* modeoverlay = NEW ModeOverlay();
		modeoverlay->Fade(nextmode_count_, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");
	}
}

void ModeMenu::NextMode()
{
	if (pushed_flag_ == false)
		return;

	nextmode_count_--;

	if (nextmode_count_ != 0)
		return;

	if (nextmode_ == HARU_A)
	{
		resource::ResourceServer::ClearModelMap();

		ModeGame* mode_game = NEW ModeGame("haru_A");
		::mode::ModeServer::GetInstance()->Add(mode_game, 0, "Game");
		::mode::ModeServer::GetInstance()->Del(this);
		appframe::ApplicationBase::GetInstance()->bgm_.Pause();
	}
	else if (nextmode_ == HARU_B)
	{
		resource::ResourceServer::ClearModelMap();

		ModeGame* mode_game = NEW ModeGame("haru_B");
		::mode::ModeServer::GetInstance()->Add(mode_game, 0, "Game");
		::mode::ModeServer::GetInstance()->Del(this);
		appframe::ApplicationBase::GetInstance()->bgm_.Pause();
	}
	else if (nextmode_ == HARU_C)
	{
		resource::ResourceServer::ClearModelMap();

		ModeGame* mode_game = NEW ModeGame("haru_C");
		::mode::ModeServer::GetInstance()->Add(mode_game, 0, "Game");
		::mode::ModeServer::GetInstance()->Del(this);
		appframe::ApplicationBase::GetInstance()->bgm_.Pause();
	}

	if (nextmode_ == TITLE)
	{
		ModeTitle* mode_title = NEW ModeTitle();
		::mode::ModeServer::GetInstance()->Add(mode_title, 0, "Title");
		::mode::ModeServer::GetInstance()->Del(this);
	}
}

bool ModeMenu::Render()
{
	::mode::ModeBase::Render();

	DrawGraph(0, 0, background_graph_, TRUE);
	DrawGraph(0, 0, cloud_graph_, TRUE);
	DrawGraph(0, 0, menu_graph_, TRUE);

	DrawRotaGraph(icon_x, icon_y, 1.0, 0.0, spring_icon_, TRUE);
	DrawRotaGraph(icon_x, icon_y, 1.0, 0.0, stage_num_graph_[cursol_], TRUE);
	DrawRotaGraph(stage_goal_x, stage_goal_y, 1.0, 0.0, stage_goal_graph_, TRUE);

	DrawRotaGraph(stage_icon_x, stage_icon_y, 0.35, 0.0, stage_graph_[cursol_], FALSE);

	return true;
}