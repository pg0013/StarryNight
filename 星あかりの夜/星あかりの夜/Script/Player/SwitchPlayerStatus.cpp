/**
 * @file    SwitchPlayerStatus.cpp
 * @brief  プレイヤーの状態遷移を行う処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include "Player.h"
#include"../Stage/Stage.h"
using namespace starrynight::player;

void Player::SwitchPlayerStatus()
{
	if (jump_flag_)
	{
		//ジャンプアニメーションを設定
		if (jump_speed_ < 0.0f)
		{
			VECTOR start_line = VAdd(position_, VGet(0, 40.0f, 0));
			VECTOR end_line = VAdd(position_, VGet(0, -40.0f, 0));

			//Navimeshとの当たり判定
			MV1_COLL_RESULT_POLY hit_jump_floor;
			hit_jump_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

			if (hit_jump_floor.HitFlag)
				status_ = STATUS::JUMP_END;
			else
				status_ = STATUS::JUMP_LOOP;
		}
	}
	else
	{
		//移動アニメーションを設定
		if (status_ == STATUS::JUMP_END &&
			anim_play_time_ < anim_total_time_ * 2 / 3)
			status_ = STATUS::JUMP_END;
		else if (utility::GetLeftStickLength() < ANALOG_MIN)
			status_ = STATUS::WAIT;
		else if (utility::GetLeftStickLength() < 0.6f)
			status_ = STATUS::WALK;
		else
			status_ = STATUS::RUN;
	}
}