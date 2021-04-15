/**
 * @file    PlayerDamageState.cpp
 * @brief  プレイヤーのダメージ状態処理
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include "PlayerDamageState.h"
#include"Player.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::player;

PlayerDamageState::PlayerDamageState()
{
	jump_speed_ = 0.0f;
	gravity_ = 0.5f;
}

PlayerDamageState::~PlayerDamageState()
{
}

void PlayerDamageState::Enter(Player& _player)
{
	_player.SetPlayerStatus(Player::STATUS::DAMAGE);
	_player.SetDamageFlag(true);

	//所持しているスターの数をリセットする
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();
	mode_game->SetPlayerStarNum(0);
}

void PlayerDamageState::Exit(Player& _player)
{
	_player.SetDamageFlag(false);
}

void PlayerDamageState::Input(Player& _player)
{
	//ダメージモーションが終了したら、待機状態へ移行
	if (_player.GetAnimPlayTime() >= _player.GetAnimTotalTime())
	{
		_player.ChangePlayerState("Wait");
	}
}

void PlayerDamageState::Update(Player& _player)
{
	Damage(_player);
}