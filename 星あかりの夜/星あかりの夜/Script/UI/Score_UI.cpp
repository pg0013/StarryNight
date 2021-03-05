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
}

Score_UI::~Score_UI()
{
}

void Score_UI::Initialize()
{
	score_base_graph_ = resource::ResourceServer::GetTexture("score_base.png");
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
}

void Score_UI::UpdatePlayerScore()
{
	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	SetPlayerScore(mode_game->GetGameScore());
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
	if (draw_scorebase_flag_)
		DrawGraph(score_base_x, score_base_y, score_base_graph_, TRUE);

	int x = static_cast<int>(position_.x);
	int interval = 67;

	for (int i = 0; i < 6; i++)
	{
		if (draw_rank_flag_[i] == false)
			continue;

		DrawRotaGraph(x, static_cast<int>(position_.y), exrate_, 0.0, score_num_graph_[rank_value_[i]], TRUE);
		x -= interval;
	}
}