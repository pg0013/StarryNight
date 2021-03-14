/**
 * @file    Score_UI.h
 * @brief  ゲームスコア表示UIクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/03
 */

#include "Score_UI.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::ui;

Score_UI::Score_UI()
{
	player_score_ = 0;
	position_ = VGet(0, 0, 0);
	exrate_ = 1.0;
	draw_scorebase_flag_ = true;
	blinking_flag_ = false;
	blink_startframe_ = 0;

	resource::ResourceServer::LoadSound("Resource/sound/star_remain.wav");
	resource::ResourceServer::LoadSound("Resource/sound/star_remain_voice.wav");
	resource::ResourceServer::LoadSound("Resource/sound/lastone_voice.wav");
}

Score_UI::~Score_UI()
{
}

void Score_UI::Initialize()
{
	score_base_graph_ = resource::ResourceServer::GetTexture("score_base.png");
	star_count_graph_ = resource::ResourceServer::GetTexture("star_count.png");

	for (int i = 0; i < 10; i++)
	{
		score_num_graph_[i] = resource::ResourceServer::GetTexture("score_number.png", i);
	}
}

void Score_UI::Terminate()
{
}

void Score_UI::Process()
{
	SetScoreRankNum();

	if (blinking_flag_ == false)
		return;

	int elapsed_frame = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - blink_startframe_;

	if (elapsed_frame == 30 ||
		elapsed_frame == 90)
	{
		se_.Load("Resource/sound/star_remain.wav");
		se_.Pan(sound::PAN_RIGHT);
		se_.Play();
	}

	if (elapsed_frame > 120)
	{
		blinking_flag_ = false;
		blink_startframe_ = 0;

		mode::ModeGame* mode_game =
			static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
		if (mode_game->GetStageStarNum() == 1)
			se_.Load("Resource/sound/lastone_voice.wav");
		else
			se_.Load("Resource/sound/star_remain_voice.wav");
		se_.Play();
	}
}

void Score_UI::UpdatePlayerScore()
{
	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	SetPlayerScore(mode_game->GetStageStarNum() - mode_game->GetPlayerStarNum());
}

void Score_UI::SetScoreRankNum()
{
	int score = player_score_;

	//桁の値を取得
	for (int i = 5; i >= 0; i--)
	{
		rank_value_[i] = score / static_cast<int>(pow(10, i));
		score -= rank_value_[i] * static_cast<int>(pow(10, i));
	}

	//最大桁数を取得
	int max_rank = 0;
	for (int i = 5; i >= 0; i--)
	{
		if (rank_value_[i] == 0)
			continue;
		else
		{
			max_rank = i;
			break;
		}
	}

	//桁を描画するか評価
	for (int i = 5; i >= 0; i--)
	{
		if (i > max_rank)
			draw_rank_flag_[i] = false;
		else
			draw_rank_flag_[i] = true;
	}
	//一の位は必ず表示
	draw_rank_flag_[0] = true;
}

void Score_UI::Render()
{
	if (blinking_flag_)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(255 * sinf(DX_PI_F / 180 * 2.125f * (::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - blink_startframe_)))));

	if (draw_scorebase_flag_)
	{
		DrawGraph(score_base_x, score_base_y, score_base_graph_, TRUE);
		DrawGraph(1300, 50, star_count_graph_, TRUE);
	}

	int x = static_cast<int>(position_.x);
	int interval = 67;

	//値を描画
	for (int i = 0; i < 6; i++)
	{
		//描画する必要のない桁は描画しない
		if (draw_rank_flag_[i] == false)
			continue;

		DrawRotaGraph(x, static_cast<int>(position_.y), exrate_, 0.0, score_num_graph_[rank_value_[i]], TRUE);
		x -= interval;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}