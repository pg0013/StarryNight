/**
 * @file    Attack.cpp
 * @brief  エネミーがプレイヤーを攻撃する処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include "Enemy.h"
#include"../Player/Player.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::enemy;

VECTOR Enemy::Attack(VECTOR& _move)
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	if (anim_play_time_ == 0.0f)
	{
		for (auto&& iter : (*mode_game->object_server_.List()))
		{
			if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
			{
				player::Player* player = static_cast<player::Player*>(iter);
				player->SetDamageAnimFlag(true);//ダメージアニメーションを再生

				//プレイヤーが星を所持していない場合、HPを減らす
				if (mode_game->GetPlayerStarNum() == 0)
					player->AddPlayerHP(-1);

				break;
			}
		}
	}

	//ダメージモーションの再生が完了したら
	if (anim_play_time_ >= anim_total_time_)
	{
		//向きを180度かえて、プレイヤーのもとを去る
		rotation_.y -= DEG2RAD(180.0f);
		move_status_ = MOVE_STATUS::ESCAPE;
		anim_status_ = ANIM_STATUS::MOVE;
		attacked_flag_ = false;

		_move = VScale(VNorm(utility::GetForwardVector(rotation_.y)), run_speed_);
	}

	return _move;
}