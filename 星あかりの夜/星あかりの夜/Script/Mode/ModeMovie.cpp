/**
 * @file    ModeMoive.h
 * @brief  オープニングのムービー再生クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/09
 */

#include "ModeMovie.h"
#include "ModeTitle.h"
#include"ModeOverlay.h"
using namespace starrynight::mode;

ModeMovie::ModeMovie()
{
	movie_ = LoadGraph("Resource/movie/op.mp4");

	pushed_flag_ = false;
	nextmode_count_ = 0;
}

ModeMovie::~ModeMovie()
{
	DeleteGraph(movie_);
}

bool ModeMovie::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	ModeOverlay* modeoverlay = NEW ModeOverlay();
	modeoverlay->Fade(60, FADE_IN);
	::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

	PlayMovieToGraph(movie_);

	return true;
}

bool ModeMovie::Terminate()
{
	::mode::ModeBase::Terminate();

	appframe::ApplicationBase::GetInstance()->se_.Fade(0.0f, 0.1f);
	return true;
}

bool ModeMovie::Process()
{
	::mode::ModeBase::Process();

	Input();
	NextMode();
	return true;
}

bool ModeMovie::Render()
{
	::mode::ModeBase::Render();

	DrawGraph(0, 0, movie_, FALSE);

	return true;
}

void ModeMovie::Input()
{
	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	if (pushed_flag_ == true)
		return;

	//何かしらのボタンが押された、もしくはムービーの再生が停止したら、タイトルへ移行
	if (trigger_key & PAD_INPUT_ALL ||
		GetMovieStateToGraph(movie_) == 0)
	{
		pushed_flag_ = true;

		nextmode_count_ = 60;

		ModeOverlay* modeoverlay = NEW ModeOverlay();
		modeoverlay->Fade(nextmode_count_, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");
	}
}

void ModeMovie::NextMode()
{
	if (pushed_flag_ == false)
		return;

	nextmode_count_--;

	if (nextmode_count_ > 0)
		return;

	PauseMovieToGraph(movie_);

	ModeTitle* mode_title = NEW ModeTitle();
	::mode::ModeServer::GetInstance()->Add(mode_title, 0, "Title");
	::mode::ModeServer::GetInstance()->Del(this);
}