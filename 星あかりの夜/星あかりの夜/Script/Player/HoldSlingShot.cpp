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

using namespace starrynight::player;

void Player::HoldSlingShot()
{
	int key = appframe::ApplicationBase::GetInstance()->GetKey();
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	status_ = STATUS::SHOOT_START;

	if (key & PAD_INPUT_4)
	{
		status_ = STATUS::SHOOT_END;
	}

	float stick_rx;
	float analog_min = 0.2f;

	//右スティックの移動量
	stick_rx = x_input.ThumbRX / THUMB_MAX;

	float rot_speed = 3.0f;
	float rot_horizon = 0.0f;

	//
	if (stick_rx > analog_min)
		rot_horizon += DEG2RAD(rot_speed) * stick_rx;
	if (stick_rx < -analog_min)
		rot_horizon -= DEG2RAD(rot_speed) * -stick_rx;

	//プレイヤーの向きを回転
	rotation_.y += rot_horizon;
}