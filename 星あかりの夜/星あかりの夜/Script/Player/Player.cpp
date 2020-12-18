/**
 * @file    Player.cpp
 * @brief  ゲームモード内のプレイヤークラス
 *
 * @author Takuya Fujisawa
 * @date    202012/16
 */
#include "Player.h"
#include"../Mode/ModeGame.h"
using namespace starrynight::player;

Player::Player()
{
	player_param_.LoadModelParameters("SDChar");
	handle_ = resource::ResourceServer::GetModelHandle("SDChar");

	move_speed_ = 300.0f;
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