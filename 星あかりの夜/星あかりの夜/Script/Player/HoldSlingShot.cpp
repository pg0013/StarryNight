/**
 * @file    HoldSlingShot.cpp
 * @brief  パチンコを構える処理
 *
 * @author Takuya Fujisawa
 * @date    202101/18
 */

#include"Player.h"
#include"../Camera/Camera.h"
#include"../Mode/ModeGame.h"
#include"../Effect/ShootChargeEffect.h"
#include"../Effect/ShootEffect.h"

using namespace starrynight::player;

void Player::HoldSlingShot()
{
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();
	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	//Yボタンが押されたら星を発射
	if (trigger_key & PAD_INPUT_4)
	{
		mode::ModeGame* modegame = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
		effect::ShootEffect* shoot_effect = NEW effect::ShootEffect();

		shoot_effect->Initialize();
		shoot_effect->PlayEffect();
		modegame->effect_server_.Add(shoot_effect);

		status_ = STATUS::SHOOT_END;
	}
	else
	{
		if (status_ != STATUS::SHOOT_END)
			status_ = STATUS::SHOOT_START;
	}

	//射撃溜めエフェクト生成
	if (status_ == STATUS::SHOOT_START &&
		anim_play_time_ == anim_total_time_)
	{
		//1フレームだけ処理を行う
		if (shoot_charge_effect_flag_)
		{
			mode::ModeGame* modegame = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
			effect::ShootChargeEffect* charge_effect = NEW effect::ShootChargeEffect();

			charge_effect->PlayEffect();
			modegame->effect_server_.Add(charge_effect);
			shoot_charge_effect_flag_ = false;
		}
	}

	float shoot_anim_end = 80.0f;

	if (status_ == STATUS::SHOOT_END &&
		anim_play_time_ == shoot_anim_end)
	{
		status_ = STATUS::WAIT;
		mode::ModeGame* modegame = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
		modegame->SetPlayerStarNum(0);
	}

	float stick_rx;

	//右スティックの移動量
	stick_rx = x_input.ThumbRX / THUMB_MAX;

	float rot_horizon = 0.0f;
	float rot_speed = 3.0f;

	if (stick_rx > ANALOG_MIN)
		rot_horizon += DEG2RAD(rot_speed) * stick_rx;
	if (stick_rx < -ANALOG_MIN)
		rot_horizon -= DEG2RAD(rot_speed) * -stick_rx;

	//プレイヤーの向きを回転
	rotation_.y += rot_horizon;
}