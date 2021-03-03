/**
 * @file    HoldSlingShot.cpp
 * @brief  パチンコを構える処理
 *
 * @author Takuya Fujisawa
 * @date    202101/18
 */

#include"Player.h"
#include"../Camera/Camera.h"
#include"../Star/Star.h"
#include"../Star/SkyStar.h"
#include"../Effect/ShootChargeEffect.h"
#include"../Effect/ShootEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::player;

void Player::HoldSlingShot()
{
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();
	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	//星との当たり判定
	MV1_COLL_RESULT_POLY hit_star;
	hit_star.HitFlag = 0;

	//射撃標準の地点への線分
	VECTOR start = camera::Camera::GetInstance()->GetPosition();
	VECTOR end = ConvScreenPosToWorldPos(
		VGet(appframe::SCREEN_WIDTH / 2.0f, appframe::SCREEN_HEIGHT / 2.0f, 1.0f));

	//星座との当たり判定をとる
	for (auto iter = mode_game->object_server_.List()->begin(); iter != mode_game->object_server_.List()->end(); iter++)
	{
		if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::SKY_STAR)
		{
			star::SkyStar* star = static_cast<star::SkyStar*>(*iter);
			hit_star = star->GetHitToSkyStar(start, end);
			break;
		}
	}

	if (hit_star.HitFlag)
	{
		hitposition = hit_star.HitPosition;

		//Yボタンが押されたら、タイミングゲージを描画
		if (trigger_key & PAD_INPUT_4)
		{
			mode_game->ui_.timing_ui_.SetSelectedStarNum(hit_star.FrameIndex);
			mode_game->ui_.timing_ui_.SetDrawTimingGuide(true);
			mode_game->ui_.shoot_ui_.SetDrawShootGuide(false);
		}
	}
	else
	{
		if (status_ != STATUS::SHOOT_END)
			status_ = STATUS::SHOOT_START;
	}

	if (mode_game->ui_.timing_ui_.GetLaunchStarShoot() == true)
	{
		effect::ShootEffect* shoot_effect = NEW effect::ShootEffect(hit_star.HitPosition);

		shoot_effect->PlayEffect();
		shoot_effect->Initialize();
		mode_game->effect_server_.Add(shoot_effect);

		//プレイヤーが連れていた星を削除する
		for (auto iter = mode_game->object_server_.List()->begin(); iter != mode_game->object_server_.List()->end(); iter++)
		{
			if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::STAGE_STAR)
			{
				star::Star* star = static_cast<star::Star*>(*iter);
				if (star->GetPlayersStarNum() > 0)
					mode_game->object_server_.Delete(*iter);
			}
		}

		status_ = STATUS::SHOOT_END;

		mode_game->ui_.timing_ui_.SetLaunchStarShoot(false);
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

	//射撃溜めエフェクト生成
	if (status_ == STATUS::SHOOT_START &&
		anim_play_time_ == 0)
	{
		//射撃標準を描画
		mode_game->ui_.shoot_ui_.SetDrawShootGuide(true);
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
	float analog_min = 0.1f;
	if (stick_rx > analog_min)
		rot_horizon += DEG2RAD(rot_speed * 0.3f) * stick_rx;
	if (stick_rx < -analog_min)
		rot_horizon -= DEG2RAD(rot_speed * 0.3f) * -stick_rx;

	//プレイヤーの向きを回転
	rotation_.y += rot_horizon;
}