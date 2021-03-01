/**
 * @file    Shoot_UI.h
 * @brief  ËŒ‚‚ÌUI•`‰æˆ—‚ğs‚¤ƒNƒ‰ƒX
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