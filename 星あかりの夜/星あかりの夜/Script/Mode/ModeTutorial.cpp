/**
 * @file    ModeTutorial.cpp
 * @brief  チュートリアル画面シーン
 *
 * @author Takuya Fujisawa
 * @date   2021/03/06
 */

#include "ModeTutorial.h"
#include"ModeOverlay.h"
#include"ModePauseMenu.h"

using namespace starrynight::mode;

ModeTutorial::ModeTutorial()
{
	//画像パラメータを読み込み
	param_.LoadImagParameters("tutorial");

	background_graph_ = resource::ResourceServer::GetTexture("tutorial_background.png");

	nextmode_count_ = 0;
	pushed_flag_ = false;
}

ModeTutorial::~ModeTutorial()
{
}

bool ModeTutorial::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	return true;
}

bool ModeTutorial::Terminate()
{
	::mode::ModeBase::Terminate();

	return true;
}

bool ModeTutorial::Process()
{
	::mode::ModeBase::Process();

	//下のレイヤーにあるゲームシーンの処理を止める
	::mode::ModeServer::GetInstance()->SkipProcessUnderLayer();

	Input();
	NextMode();

	return true;
}

void ModeTutorial::Input()
{
	if (pushed_flag_ == true)
		return;

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	//Aボタンでゲームに進む
	if (trigger_key & PAD_INPUT_2)
	{
		pushed_flag_ = true;

		nextmode_count_ = 1;

		appframe::ApplicationBase::GetInstance()->se_.Load("Resource/sound/se2.wav");
		appframe::ApplicationBase::GetInstance()->se_.Pitch(0.9f);
		appframe::ApplicationBase::GetInstance()->se_.Play();
	}
}

void ModeTutorial::NextMode()
{
	if (pushed_flag_ == false)
		return;

	nextmode_count_--;

	if (nextmode_count_ > 0)
		return;

	::mode::ModeServer::GetInstance()->Del(this);
}

bool ModeTutorial::Render()
{
	::mode::ModeBase::Render();

	DrawGraph(0, 0, background_graph_, TRUE);

	return true;
}