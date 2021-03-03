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
		DrawRotaGraph(hp_x + 120*i, hp_y, 0.5, 0.0, hp_graph_, TRUE);
	}
}

void Hp_UI::UpdatePlayerHP()
{
	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	for (auto iter = mode_game->object_server_.List()->begin(); iter != mode_game->object_server_.List()->end(); iter++)
	{
		if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>((*iter));
			player_hp_ = player->GetPlayerHP();
			break;
		}
	}
}