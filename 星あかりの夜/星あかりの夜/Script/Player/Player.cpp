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
	player_param_.LoadModelParameters("Player");
	model_handle_ = resource::ResourceServer::GetModelHandle("SDChar");
}

Player::~Player()
{

}

void Player::Initialize()
{

}

void Player::Process()
{

}

void Player::Render()
{
	starrynight::mode::ModeGame* mode_game = static_cast<starrynight::mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	//MV1SetPosition(model_handle_, mode_game->camera_.GetTarget());
	MV1DrawModel(model_handle_);
}

void Player::Input()
{

}