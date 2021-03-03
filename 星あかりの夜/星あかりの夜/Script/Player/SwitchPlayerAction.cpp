/**
 * @file    SwitchPlayerAction.cpp
 * @brief  プレイヤーのアクション切り替えを行う処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include"Player.h"
#include"../Mode/ModeGame.h"
using namespace starrynight::player;

void Player::SwitchPlayerAction()
{
	if (damage_flag_)
	{
		Damage();
		return;
	}

	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	int key = appframe::ApplicationBase::GetInstance()->GetKey();

	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	if (x_input.RightTrigger == 255 &&
		jump_flag_ == false &&
		mode_game->GetPlayerStarNum() > 0)
	{
		//射撃アクションを行う
		camera::Camera::GetInstance()->SetStatus(camera::Camera::STATUS::SHOOT);
		HoldSlingShot();
	}
	else
	{
		//移動、ジャンプアクションを行う
		camera::Camera::GetInstance()->SetStatus(camera::Camera::STATUS::MOVE);
		mode_game->ui_.shoot_ui_.SetDrawShootGuide(false);
		shoot_charge_effect_flag_ = true;

		Move();
		Jump();

		SwitchPlayerStatus();
		DecideForwardDirection();
	}
}