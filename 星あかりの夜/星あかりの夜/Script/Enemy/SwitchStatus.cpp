/**
 * @file    SwitchStatus.cpp
 * @brief  プレイヤーの状態遷移処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include "Enemy.h"
#include"../Player/Player.h"
#include"../Effect/DamageEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::enemy;

void Enemy::SwitchStatus(ANIM_STATUS _old_status)
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	int elapsed_frame = mode_game->GetModeCount() - start_frame_;
	bool player_damaged = false;
	for (auto iter = mode_game->object_server_.List()->begin(); iter != mode_game->object_server_.List()->end(); iter++)
	{
		if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(*iter);
			player_damaged = player->GetDamageFlag();
			break;
		}
	}

	MOVE_STATUS  old_status = move_status_;

	//索敵状態であれば、経過時間によって待機と直進を交互に繰り返す
	if (elapsed_frame % (60 * (GetRand(5) + 1)) == 0)
	{
		if (old_status == MOVE_STATUS::STRAIGHT)
		{
			move_status_ = MOVE_STATUS::WAIT;
			//回転方向をランダムに変える
			random_rot_direction_ *= powf(-1.0f, static_cast<float>(GetRand(1) + 1));
		}
		if (old_status == MOVE_STATUS::WAIT)
		{
			move_status_ = MOVE_STATUS::STRAIGHT;
		}
	}

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	float player_distance = VSize(VSub(player_position, position_));
	float player_distance_y = abs(player_position.y - position_.y);

	//追跡距離範囲内であれば、追跡モードに移行
	//逃げるモードの時にはプレイヤーから離れるために追跡しない
	if (player_distance < detect_length_ &&
		move_status_ != MOVE_STATUS::ESCAPE)
	{
		if (player_distance_y < 200 &&
			player_damaged == false)
			move_status_ = MOVE_STATUS::TRACKING;
		else
			move_status_ = MOVE_STATUS::WAIT;
	}

	//攻撃できる距離まで近づけば、攻撃モードに移行
	if (player_distance <= attack_length_ &&
		move_status_ != MOVE_STATUS::ESCAPE)
	{
		move_status_ = MOVE_STATUS::ATTACK;

		//1フレームだけ処理を行うためのフラグ
		if (attacked_flag_ == false)
		{
			for (auto iter = mode_game->object_server_.List()->begin(); iter != mode_game->object_server_.List()->end(); iter++)
			{
				//プレイヤーをエネミーの方に向ける
				if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
				{
					player::Player* player = static_cast<player::Player*>(*iter);
					player->SetRotation(VGet(0.0f, rotation_.y + DEG2RAD(180.0f), 0.0f));
					player->SetDamageFlag(true);
					break;
				}
			}
			//プレイヤーのダメージエフェクトを描画
			effect::DamageEffect* damage_effect = NEW effect::DamageEffect();
			VECTOR damage_pos = VScale(VAdd(player_position, position_), 0.5f);
			damage_pos = VAdd(damage_pos, VGet(0, 50, 0));

			damage_effect->SetPosition(damage_pos);
			damage_effect->SetRotation(VGet(0, 0, 0));
			damage_effect->Initialize();

			damage_effect->PlayEffect();
			mode_game->effect_server_.Add(damage_effect);

			se_.Load("Resource/sound/enemy_se2.wav");
			se_.Play();
			se_.Fade(0.0f, 1.5f);

			attacked_flag_ = true;
		}
	}
}