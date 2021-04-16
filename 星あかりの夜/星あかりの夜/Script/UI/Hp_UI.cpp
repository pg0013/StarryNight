/**
 * @file    Hp_UI.cpp
 * @brief  HP表示クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/03
 */

#include "Hp_UI.h"
#include"../Mode/ModeGame.h"
#include"../Player/Player.h"

using namespace starrynight::ui;

Hp_UI::Hp_UI()
{
	player_hp_ = 0;
	interval = 80;
}

Hp_UI::~Hp_UI()
{
}

void Hp_UI::Initialize()
{
	hp_graph_ = resource::ResourceServer::GetTexture("hp.png");
}

void Hp_UI::Terminate()
{
}

void Hp_UI::Process()
{
	//hpを更新
	UpdatePlayerHP();
}

void Hp_UI::Render()
{
	for (int i = 0; i < player_hp_; i++)
	{
		DrawRotaGraph(interval + 120 * i, interval, 0.5, 0.0, hp_graph_, TRUE);
	}
}

void Hp_UI::UpdatePlayerHP()
{
	std::shared_ptr<mode::ModeGame> mode_game =
		mode::ModeGame::GetModeGame();

	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			std::shared_ptr<player::Player> player = std::dynamic_pointer_cast<player::Player>(iter);
			player_hp_ = player->GetPlayerHP();
			break;
		}
	}
}