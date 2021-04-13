/**
 * @file    StarAction.cpp
 * @brief  スターの待機処理
 *
 * @author Takuya Fujisawa
 * @date   2021/03/13
 */

#include "Star.h"
#include"../Effect/GetStarEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::star;

void Star::Wait()
{
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR distance = VSub(VAdd(player_position, VGet(0, 80, 0)), VAdd(position_, VGet(0, 40, 0)));

	float length = VSize(distance);
	float detect_length = 1000.0f;//検出範囲
	float touch_length = 100.0f;//取得範囲

	//プレイヤーとの距離が検出範囲であれば、飛び跳ねる
	if (length < detect_length)
	{
		if (position_.y <= ground_position_y_)
		{
			position_.y = ground_position_y_;
			jump_speed_ = jump_height_;

			se_.Load("Resource/sound/star_wait.wav");
			//画面のスクリーン座標によって音の左右バランスを設定
			se_.Pan(static_cast<int>(utility::GetScreenPosFromWorldPos(position_).x));
			//プレイヤーとの距離で音の音量を設定
			float volume = 1.0f - VSize(VSub(MV1GetPosition(resource::ResourceServer::GetModelHandle("player")), position_)) / detect_length;
			se_.SetVolume(volume);
			se_.Play();
		}

		position_.y += jump_speed_;
		jump_speed_ -= gravity_;
	}
	else
	{
		if (jump_speed_ >= -jump_height_)
		{
			position_.y += jump_speed_;
			jump_speed_ -= gravity_;
		}
		else
		{
			position_.y = ground_position_y_;
		}
	}

	//プレイヤーとの距離が取得範囲ないであれば、followモードへ遷移
	if (length < touch_length)
	{
		status_ = STATUS::FOLLOW;

		//取得したスターの数を増やす
		mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();
		mode_game->AddPlayerStarNum();
		star_num_ = mode_game->GetPlayerStarNum();

		VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));
		player_position = VAdd(player_position, VGet(0.0f, 50.0f, 0.0f));
		//プレイヤーの後ろに星を移動
		position_ = VAdd(player_position, VScale(VNorm(utility::GetForwardVector(player_rotation.y)), -0.5f * follow_interval_));

		//星獲得エフェクトを生成
		effect::GetStarEffect* getstar_effect = NEW effect::GetStarEffect();
		getstar_effect->SetPosition(player_position);
		getstar_effect->Initialize();
		getstar_effect->PlayEffect();
		mode_game->effect_server_.Add(getstar_effect);

		//連れている星を見ると回転が少しずれているように、回転値を設定する
		rotation_.y = DEG2RAD(45.0f * star_num_);

		se_.Load("Resource/sound/getstar.wav");
		se_.Play();
	}

	rotation_.y += DEG2RAD(rot_speed_);
}