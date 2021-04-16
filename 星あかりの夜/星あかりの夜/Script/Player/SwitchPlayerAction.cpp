/**
 * @file    SwitchPlayerAction.cpp
 * @brief  プレイヤーのアクション切り替えを行う処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include"Player.h"
#include"../Stage/Stage.h"
#include"../Mode/ModeGame.h"
using namespace starrynight::player;

void Player::SwitchPlayerAction()
{
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	//星座カメラモードであれば、プレイヤーの処理を行わない
	if (camera::Camera::GetInstance()->GetStatus() == camera::Camera::STATUS::SKYSTAR)
		return;

	//状態切り替え処理
	player_state_->Input(*this);
	//現在の状態の更新処理
	player_state_->Update(*this);
}