/**
 * @file    ModeLoading.cpp
 * @brief  ローディングモードクラスの処理
 *
 * @author Takuya Fujisawa
 * @date   2021/03/13
 */

#include "ModeLoading.h"
#include"ModeOverlay.h"

using namespace starrynight::mode;

ModeLoading::ModeLoading()
{
	//画像パラメータを読み込み
	param_.LoadImagParameters("loading");

	//画像ハンドルを記録
	tutorial_graph_ = resource::ResourceServer::GetTexture("loading.png");

	pushed_flag_ = false;
	nextmode_count_ = 0;
}

ModeLoading::~ModeLoading()
{
}

bool ModeLoading::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	ModeOverlay* modeoverlay = NEW ModeOverlay();
	modeoverlay->Fade(30, FADE_IN);
	::mode::ModeServer::GetInstance()->Add(modeoverlay, 11, "Overlay");

	appframe::ApplicationBase::GetInstance()->se_.Load("Resource/sound/se1.wav");
	appframe::ApplicationBase::GetInstance()->se_.SetVolume(1.0f);
	appframe::ApplicationBase::GetInstance()->se_.Play();
	appframe::ApplicationBase::GetInstance()->se_.Fade(0.0f, 0.5f);
	appframe::ApplicationBase::GetInstance()->bgm_.Fade(0.0f, 0.5f);

	return true;
}

bool ModeLoading::Terminate()
{
	::mode::ModeBase::Terminate();

	return true;
}

bool ModeLoading::Process()
{
	::mode::ModeBase::Process();

	NextMode();
	return true;
}

bool ModeLoading::Render()
{
	::mode::ModeBase::Render();

	DrawGraph(0, 0, tutorial_graph_, FALSE);

	return true;
}

void ModeLoading::SetNextMode()
{
	pushed_flag_ = true;

	nextmode_count_ = 30;

	ModeOverlay* modeoverlay = NEW ModeOverlay();
	modeoverlay->Fade(nextmode_count_, FADE_OUT);
	::mode::ModeServer::GetInstance()->Add(modeoverlay, 11, "Overlay");
}

void ModeLoading::NextMode()
{
	if (pushed_flag_ == false)
		return;

	nextmode_count_--;

	if (nextmode_count_ > 0)
		return;

	::mode::ModeServer::GetInstance()->Del(this);
}