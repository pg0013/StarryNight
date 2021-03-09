/**
 * @file    TimingGame_UI.cpp
 * @brief  射撃時のタイミングゲームUIを表示するクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/01
 */

#include "TimingGame_UI.h"
#include"../Mode/ModeGame.h"
#include"../Effect/TimingRankEffect.h"

using namespace starrynight::ui;

TimingGame_UI::TimingGame_UI()
{
	draw_timing_guide_ = false;
	launch_star_shoot_ = false;
	shrink_circle_flag_ = true;
	timing_exrate_ = 1.0f;
	presssed_frame_ = 0;
	select_star_frame_ = 0;
}

TimingGame_UI::~TimingGame_UI()
{
}

void TimingGame_UI::Initialize()
{
	circle_guide_ = resource::ResourceServer::GetTexture("circle_green.png");
	timing_circle_ = resource::ResourceServer::GetTexture("circle_white.png");
	timing_judge_[0] = resource::ResourceServer::GetTexture("excellent.png");
	timing_judge_[1] = resource::ResourceServer::GetTexture("good.png");
	timing_judge_[2] = resource::ResourceServer::GetTexture("bad.png");
}

void TimingGame_UI::Terminate()
{
}

void TimingGame_UI::Process()
{
	if (draw_timing_guide_ == false)
		return;

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	//星が選択されてからもう一度Yボタンが押されたら、円の収縮を停止する
	if (trigger_key & PAD_INPUT_4 &&
		select_star_frame_ != ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount())
	{
		//円の縮小を停止
		shrink_circle_flag_ = false;
		presssed_frame_ = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount();
	}

	int elapsed_frame = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - presssed_frame_;

	if (elapsed_frame == 30 &&
		presssed_frame_ != 0)
	{
		//タイミングゲームUIの描画を停止し、星を発射するフラグを設定
		VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
		player_position = VAdd(player_position, VGet(0.0f, 50.0f, 0.0f));
	}

	if (elapsed_frame > 60 &&
		presssed_frame_ != 0)
	{
		draw_timing_guide_ = false;
		SetLaunchStarShoot(true);

		//タイミングからスコアを計算
		CalcurateScore();

		//拡大率とボタンを押したフレームを初期化
		timing_exrate_ = 1.0;
		presssed_frame_ = 0;
		shrink_circle_flag_ = true;
		return;
	}

	if (shrink_circle_flag_ == false)
		return;

	//選ばれた星によって縮小の速度を変える
	switch (selected_star_num_)
	{
	case 1:	timing_exrate_ -= firststar_scale_speed_; break;
	case 2:	timing_exrate_ -= secondstar_scale_speed_; break;
	case 3:	timing_exrate_ -= thirdstar_scale_speed_; break;
	}

	//ボタンを押さなかったら、拡大率0でボタンを押したことにする
	if (timing_exrate_ < 0)
	{
		timing_exrate_ = 0;
		shrink_circle_flag_ = false;
		presssed_frame_ = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount();
	}
}

void TimingGame_UI::Render()
{
	if (draw_timing_guide_ == false)
		return;

	double angle = 0.0;
	double exrate = 1.0f;

	DrawRotaGraph(appframe::SCREEN_WIDTH / 2, appframe::SCREEN_HEIGHT / 2, exrate, angle, circle_guide_, TRUE);
	DrawRotaGraph(appframe::SCREEN_WIDTH / 2, appframe::SCREEN_HEIGHT / 2, timing_exrate_, angle, timing_circle_, TRUE);

	if (shrink_circle_flag_)
		return;

	//判定表示を描画
	DrawRotaGraph(appframe::SCREEN_WIDTH / 2 + static_cast<int>(500 * timing_exrate_) + 250, appframe::SCREEN_HEIGHT / 2,
		exrate, angle, timing_judge_[static_cast<int>(CheckTiming())], TRUE);
}

void TimingGame_UI::CalcurateScore()
{
	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	double score = static_cast<double>(mode_game->GetGameScore());
	int regulations_score = mode_game->GetStageRegulationsScore();
	double star_rate = static_cast<double>(mode_game->GetPlayerStarNum()) / static_cast<double>(mode_game->GetStageStarNum());
	star_rate *= 10;
	star_rate = floor(star_rate);
	star_rate /= 10;

	//一度にすべてのスターを集めたら規定スコアの整数倍にする
	if (mode_game->GetPlayerStarNum() == mode_game->GetStageStarNum())
	{
		switch (selected_star_num_)
		{
		case 1:
			if (CheckTiming() == TIMING_STATUS::EXCELLENT)
			{
				score = regulations_score * 5.0;
				//スコア評価を設定
				mode_game->SetScoreRank(mode::ModeGame::SCORE_RANK::HIGH);
			}
			else if (CheckTiming() == TIMING_STATUS::GOOD)
			{
				score = regulations_score * 4.0;
				//スコア評価を設定
				mode_game->SetScoreRank(mode::ModeGame::SCORE_RANK::MIDDLE);
			}
			else
				score = regulations_score * 3.0;
			break;
		case 2:
			if (CheckTiming() == TIMING_STATUS::EXCELLENT)
			{
				score = regulations_score * 4.0;
				//スコア評価を設定
				mode_game->SetScoreRank(mode::ModeGame::SCORE_RANK::HIGH);
			}
			else if (CheckTiming() == TIMING_STATUS::GOOD)
			{
				score = regulations_score * 2.0;
				//スコア評価を設定
				mode_game->SetScoreRank(mode::ModeGame::SCORE_RANK::MIDDLE);
			}
			else
				score = regulations_score * 2.0;
			break;
		case 3:
			if (CheckTiming() == TIMING_STATUS::EXCELLENT)
			{
				score = regulations_score * 3.0;
				//スコア評価を設定
				mode_game->SetScoreRank(mode::ModeGame::SCORE_RANK::HIGH);
			}
			else if (CheckTiming() == TIMING_STATUS::GOOD)
			{
				score = regulations_score * 1.5;
				//スコア評価を設定
				mode_game->SetScoreRank(mode::ModeGame::SCORE_RANK::MIDDLE);
			}
			else
				score = regulations_score * 1.5;
			break;
		}
	}
	else
	{
		switch (selected_star_num_)
		{
		case 1:
			if (CheckTiming() == TIMING_STATUS::EXCELLENT)
				score = regulations_score * (star_rate + 0.08);
			else if (CheckTiming() == TIMING_STATUS::GOOD)
				score = regulations_score * star_rate;
			else
				score = regulations_score * (star_rate + 0.05);
			break;
		case 2:
			if (CheckTiming() == TIMING_STATUS::EXCELLENT)
				score = regulations_score * (star_rate + 0.04);
			else if (CheckTiming() == TIMING_STATUS::GOOD)
				score = regulations_score * (star_rate - 0.04);
			else
				score = regulations_score * (star_rate - 0.07);
			break;
		case 3:
			if (CheckTiming() == TIMING_STATUS::EXCELLENT)
				score = regulations_score * star_rate;
			else if (CheckTiming() == TIMING_STATUS::GOOD)
				score = regulations_score * (star_rate - 0.08);
			else
				score = regulations_score * (star_rate - 0.1);
			break;
		}

		//星を5割以上集めてタイミングが合えば、スコア評価のMIDDLEが確定
		if (mode_game->GetPlayerStarNum() >= mode_game->GetStageStarNum() / 2)
		{
			if (CheckTiming() == TIMING_STATUS::EXCELLENT)
				mode_game->SetScoreRank(mode::ModeGame::SCORE_RANK::MIDDLE);
		}
	}

	mode_game->AddGameScore(static_cast<int>(score));
}

TimingGame_UI::TIMING_STATUS TimingGame_UI::CheckTiming()
{
	if (timing_exrate_ <= 0.43 &&
		timing_exrate_ >= 0.37)
	{
		return TIMING_STATUS::EXCELLENT;
	}
	else if (timing_exrate_ <= 0.6 &&
		timing_exrate_ > 0.43)
	{
		return TIMING_STATUS::GOOD;
	}
	else if (timing_exrate_ < 1.0 &&
		timing_exrate_ > 0.6)
	{
		return TIMING_STATUS::BAD;
	}
	else
	{
		return TIMING_STATUS::BAD;
	}
}