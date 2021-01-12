/**
 * @file    Player.cpp
 * @brief  ゲームモード内のプレイヤークラス
 *
 * @author Takuya Fujisawa
 * @date    202012/16
 */
#include "Player.h"
#include"../Mode/ModeGame.h"
#include<vector>
using namespace starrynight::player;

Player::Player()
{
	player_param_.LoadModelParameters("player");
	std::vector<std::string> param = { "move_speed","rot_speed" };
	player_param_.LoadPlayerParameters(param);

	handle_ = resource::ResourceServer::GetModelHandle("player");

	move_speed_ = player_param_.GetPlayerParam("move_speed");
	rot_speed_ = player_param_.GetPlayerParam("rot_speed");

	status_ = STATUS::WAIT;
	anim_attach_index_ = -1;
	old_anim_attach_index_ = -1;
	anim_rate_ = 0.0f;

	Initialize();
}

Player::~Player()
{

}

void Player::Initialize()
{
	ObjectBase::Initialize();
}

void Player::Input()
{

}

void Player::Process()
{
	STATUS old_status = status_;
	Move();
	SwitchAnimation(old_status);

}

void Player::Render()
{
	if (anim_rate_ >= 1.0f)
		MV1SetAttachAnimTime(handle_, anim_attach_index_, anim_play_time_);
	else
	{
		MV1SetAttachAnimBlendRate(handle_, old_anim_attach_index_, 1.0f - anim_rate_);
		MV1SetAttachAnimBlendRate(handle_, anim_attach_index_, anim_rate_);
	}

	MV1SetPosition(handle_, position_);
	MV1SetRotationXYZ(handle_, rotation_);
	MV1DrawModel(handle_);
}