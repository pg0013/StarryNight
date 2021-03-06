/**
 * @file    ModeGameClear.h
 * @brief  ゲームクリアシーン
 *
 * @author Takuya Fujisawa
 * @date   2021/03/05
 */

#include "ModeGameClear.h"
#include"ModeOverlay.h"
//#include"ModeGame.h"
#include"ModeMenu.h"
#include"ModeTitle.h"

using namespace starrynight::mode;

ModeGameClear::ModeGameClear(int _score)
{
	score_ui_.SetPlayerScore(_score);
	score_ui_.SetPosition(VGet(1500, 540, 0));
	score_ui_.SetDrawScoreBaseFlag(false);

	//画像パラメータを読み込み
	param_.LoadImagParameters("gameclear");

	background_graph_ = resource::ResourceServer::GetTexture("background.png");
	charactor_graph_ = resource::ResourceServer::GetTexture("charactor.png");
	return_title_graph_ = resource::ResourceServer::GetTexture("return_title.png");
	return_titlebase_graph_ = resource::ResourceServer::GetTexture("return_titlebase.png");
	return_graph_ = resource::ResourceServer::GetTexture("return.png");
	return_base_graph_ = resource::ResourceServer::GetTexture("return_base.png");

	cursol_ = 0;
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

	return true;
}

bool ModeGameClear::Terminate()
{
	::mode::ModeBase::Terminate();

	score_ui_.Terminate();

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
	}
}

void ModeGameClear::NextMode()
{
	if (pushed_flag_ == false)
		return;

	nextmode_count_--;

	if (nextmode_count_ != 0)
		return;

	if (cursol_ == RETURN_MENU)
	{
		resource::ResourceServer::ClearMap();

		ModeMenu* mode_menu = NEW ModeMenu();
		::mode::ModeServer::GetInstance()->Add(mode_menu, 0, "Menu");
		::mode::ModeServer::GetInstance()->Del(this);
	}

	if (cursol_ == RETURN_TITLE)
	{
		resource::ResourceServer::ClearMap();

		ModeTitle* mode_title = NEW ModeTitle();
		::mode::ModeServer::GetInstance()->Add(mode_title, 0, "Title");
		::mode::ModeServer::GetInstance()->Del(this);
	}
}

bool ModeGameClear::Render()
{
	::mode::ModeBase::Render();

	DrawGraph(0, 0, background_graph_, TRUE);
	DrawGraph(0, 0, charactor_graph_, TRUE);

	//タイトルへ戻る
	DrawRotaGraph(1080, 700, 0.5, 0.0, return_titlebase_graph_, TRUE);

	if (cursol_ == RETURN_TITLE)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 10 * GetModeCount()))));
	DrawRotaGraph(1080, 700, 0.5, 0.0, return_title_graph_, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//戻る
	DrawRotaGraph(1780, 800, 0.5, 0.0, return_base_graph_, TRUE);

	if (cursol_ == RETURN_MENU)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 10 * GetModeCount()))));
	DrawRotaGraph(1780, 800, 0.5, 0.0, return_graph_, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	score_ui_.Render();

	return true;
}