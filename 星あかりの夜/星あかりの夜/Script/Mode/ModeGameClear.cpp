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
#include"ModeLoading.h"
#include<algorithm>

using namespace starrynight::mode;

ModeGameClear::ModeGameClear(const int& _score, const std::string& _stage_name)
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

	stage_name_ = _stage_name;

	se_.Load("Resource/sound/success_voice.wav");
}

ModeGameClear::~ModeGameClear()
{
	if (se_.CheckIsRunning())
	{
		se_.Pause();
		se_.Destroy();
	}
}

bool ModeGameClear::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	std::shared_ptr<ModeOverlay> modeoverlay = std::make_shared<ModeOverlay>();
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

	if (GetModeCount() == 60)
	{
		//やったね大成功を再生
		se_.Play();
	}

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

		std::shared_ptr<ModeOverlay> modeoverlay = std::make_shared<ModeOverlay>();
		modeoverlay->Fade(nextmode_count_, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");

		if (cursol_ == NEXT_GAME)
		{
			std::shared_ptr<ModeLoading> mode_loading = std::make_shared<ModeLoading>();
			::mode::ModeServer::GetInstance()->Add(mode_loading, 10, "Loading");
		}
		else
		{
			appframe::ApplicationBase::GetInstance()->se_.Load("Resource/sound/se1.wav");
			appframe::ApplicationBase::GetInstance()->se_.SetVolume(1.0f);
			appframe::ApplicationBase::GetInstance()->se_.Play();
			appframe::ApplicationBase::GetInstance()->se_.Fade(0.0f, 0.8f);

			appframe::ApplicationBase::GetInstance()->bgm_.Fade(0.0f, 0.8f);
		}
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

		std::shared_ptr<ModeGame> mode_game = nullptr;

		//プレイしたステージによって、次に進むステージを設定する
		if (std::equal(stage_name_.begin(), stage_name_.end(), "haru_A"))
		{
			mode_game = std::make_shared<ModeGame>("haru_B");
		}
		else if (std::equal(stage_name_.begin(), stage_name_.end(), "haru_B"))
		{
			mode_game = std::make_shared<ModeGame>("haru_C");
		}
		else
		{
			mode_game = std::make_shared<ModeGame>("haru_A");
		}

		::mode::ModeServer::GetInstance()->Add(mode_game, 0, "Game");
		::mode::ModeServer::GetInstance()->Del(shared_from_this());
	}

	if (cursol_ == RETURN_TITLE)
	{
		std::shared_ptr<ModeTitle> mode_title = std::make_shared<ModeTitle>();
		::mode::ModeServer::GetInstance()->Add(mode_title, 0, "Title");
		::mode::ModeServer::GetInstance()->Del(shared_from_this());
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
		//選択されたら点滅する
		if (pushed_flag_ == false)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 2.125f * GetModeCount()))));

		DrawRotaGraph(1380, 880, 1.0, 0.0, return_base_graph_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//タイトルへ戻る
	DrawRotaGraph(525, 850, 1.0, 0.0, next_graph_, TRUE);

	if (cursol_ == NEXT_GAME)
	{
		//選択されたら点滅する
		if (pushed_flag_ == false)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 2.125f * GetModeCount()))));

		DrawRotaGraph(550, 880, 1.0, 0.0, next_base_graph_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	score_ui_.Render();

	return true;
}