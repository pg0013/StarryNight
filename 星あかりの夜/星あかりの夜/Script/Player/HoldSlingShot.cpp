/**
 * @file    HoldSlingShot.cpp
 * @brief  パチンコを構える処理
 * 
 * @author Takuya Fujisawa
 * @date    202101/18
 */

#include"Player.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::player;

void Player::HoldSlingShot()
{
	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	if (trigger_key & PAD_INPUT_4)
	{

	}
}