/**
 * @file    Star.cpp
 * @brief  ステージ内のスター挙動実装クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/01/23
 */

#include "Star.h"
#include"../Player/Player.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::star;

Star::Star()
{
	status_ = STATUS::WAIT;
	rot_speed_ = 5.0f;
	jump_height_ = 4.5f;
	jump_speed_ = -jump_height_;
	gravity_ = 0.1f;
	star_num_ = 0;
	follow_interval_ = 100.0f;
	old_player_position_ = { 0,0,0 };
	ground_position_y_ = 0.0f;
}

Star::~Star()
{
}

void Star::Initialize()
{
	position_ = MV1GetPosition(handle_);
	rotation_ = MV1GetRotationXYZ(handle_);
	ground_position_y_ = position_.y;
}

void Star::Process()
{
	switch (status_)
	{
	case starrynight::star::Star::STATUS::WAIT:
		Wait();
		break;
	case starrynight::star::Star::STATUS::FOLLOW:
		Follow();
		break;
	case starrynight::star::Star::STATUS::DIFFUSION:
		Diffusion();
		break;
	}
}

int Star::IsStageStarDraw()
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	bool star_draw_flag = true;

	//プレイヤーが射撃状態かどうか取得
	for (auto iter = mode_game->object_server_.List()->begin(); iter != mode_game->object_server_.List()->end(); iter++)
	{
		if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(*iter);
			star_draw_flag = player->GetPlayerSlingShotStatus();
			break;
		}
	}

	if (star_draw_flag)
		return FALSE;
	else
		return TRUE;
}

void Star::Render()
{
	MV1SetPosition(handle_, position_);
	MV1SetRotationXYZ(handle_, rotation_);

	MV1SetVisible(handle_, IsStageStarDraw());
	MV1DrawModel(handle_);

#ifdef DEBUG_FUNCTION
	DrawLine3D(position_, VAdd(position_, VGet(0, 2000, 0)), GetColor(255, 255, 0));
#endif // debug_
}