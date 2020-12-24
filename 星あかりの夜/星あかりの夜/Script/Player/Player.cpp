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
	Move();
}

void Player::Render()
{
	MV1SetPosition(handle_, position_);
	MV1SetRotationXYZ(handle_, rotation_);
	MV1DrawModel(handle_);
}