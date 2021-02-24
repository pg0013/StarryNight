/**
 * @file    Shoot_UI.h
 * @brief  射撃時のUI描画処理を行うクラス
 *
 * @author Takuya Fujisawa
 * @date    202102/08
 */

#include "Shoot_UI.h"
#include"../Mode/ModeGame.h"
#include"../Player/Player.h"

using namespace starrynight::ui;

Shoot_UI::Shoot_UI()
{
	draw_shoot_guide_ = false;
}

Shoot_UI::~Shoot_UI()
{
}

void Shoot_UI::Initialize()
{
	shoot_guide_ = resource::ResourceServer::GetTexture("shoot_guide.png");
}

void Shoot_UI::Terminate()
{
}

void Shoot_UI::Process()
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	for (auto iter = mode_game->object_server_.List()->begin(); iter != mode_game->object_server_.List()->end(); iter++)
	{
		if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(*iter);

			//射撃標準を表示
			if (player->GetPlayerStatus() == player::Player::STATUS::SHOOT_START)
			{
				mode_game->ui_.shoot_ui_.SetDrawShootGuide(true);
			}
			else
			{
				mode_game->ui_.shoot_ui_.SetDrawShootGuide(false);
			}
			break;
		}
	}

}

void Shoot_UI::Render()
{
	if (draw_shoot_guide_)
	{
		double angle = 0.0;
		double exrate = 1.0f;
		DrawRotaGraph(appframe::SCREEN_WIDTH / 2, appframe::SCREEN_HEIGHT / 2, exrate, angle, shoot_guide_, TRUE);
	}
}