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
#include"../Mode/ModeOverlay.h"


using namespace starrynight::player;

void Player::HoldSlingShot()
{
	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	//射撃構え処理
	SlingShotStance();

	//星との当たり判定
	MV1_COLL_RESULT_POLY hit_star = CheckHitStar();

	//星選択前であれば、トリガーを離すと射撃状態を解除
	if (x_input.RightTrigger != 255 &&
		selected_skystar_flag_ == false)
		slingshot_flag_ = false;

	//星と標準があっていないので、次の処理に進まない
	if (hit_star.HitFlag == 0)
		return;

	//Yボタンが押されたら、タイミングゲージを描画
	if (trigger_key & PAD_INPUT_4)
	{
		mode_game->ui_.timing_ui_.SetSelectedStarNum(hit_star.FrameIndex);
		mode_game->ui_.shoot_ui_.SetDrawShootGuide(false);
		mode_game->ui_.timing_ui_.SetDrawTimingGuide(true);
		selected_skystar_flag_ = true;
	}

	//星発射エフェクトを生成
	Launch_Star(hit_star.HitPosition);

	if (mode_game->IsClearStage() == true &&
		anim_play_time_ == 70.0f)
	{
		mode::ModeOverlay* modeoverlay = NEW mode::ModeOverlay();
		modeoverlay->Fade(11, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");
	}


	//射撃状態の終了
	float shoot_anim_end = 80.0f;
	if (anim_play_time_ < shoot_anim_end)
		return;

	if (mode_game->IsClearStage() == true)
	{
		camera::Camera::GetInstance()->SkyStarCameraInit();
		camera::Camera::GetInstance()->SetStatus(camera::Camera::STATUS::SKYSTAR);
		mode_game->SetNextMode(420,40,0);
	}
	else
	{
		status_ = STATUS::WAIT;
		slingshot_flag_ = false;
		selected_skystar_flag_ = false;
		mode_game->SetPlayerStarNum(0);
	}
}

void Player::SlingShotStance()
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	if (status_ == STATUS::SHOOT_END)
		return;

	status_ = STATUS::SHOOT_START;

	if (anim_play_time_ == 0)
	{
		//射撃標準を描画
		mode_game->ui_.shoot_ui_.SetDrawShootGuide(true);
	}

	//射撃溜めエフェクト生成
	if (anim_play_time_ == anim_total_time_)
	{
		//1フレームだけ処理を行う
		if (shoot_charge_effect_flag_)
		{
			effect::ShootChargeEffect* charge_effect = NEW effect::ShootChargeEffect();

			charge_effect->PlayEffect();
			mode_game->effect_server_.Add(charge_effect);
			shoot_charge_effect_flag_ = false;
		}
	}

	//射撃標準の回転を行う
	if (selected_skystar_flag_ == false)
		SetShootRotation();
}

MV1_COLL_RESULT_POLY Player::CheckHitStar()
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

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

	return hit_star;
}

void Player::Launch_Star(VECTOR _star_position)
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	if (mode_game->ui_.timing_ui_.GetLaunchStarShoot() == false)
		return;

	//射撃エフェクトの生成
	effect::ShootEffect* shoot_effect = NEW effect::ShootEffect(_star_position);

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

void Player::SetShootRotation()
{
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	//右スティックの移動量
	float stick_rx = x_input.ThumbRX / THUMB_MAX;

	float rot_horizon = 0.0f;
	float rot_speed = 3.0f;
	float analog_min = 0.1f;

	//射撃の標準は通常より細かく移動できるように、速度を遅くする
	if (stick_rx > analog_min)
		rot_horizon += DEG2RAD(rot_speed * 0.3f) * stick_rx;
	if (stick_rx < -analog_min)
		rot_horizon -= DEG2RAD(rot_speed * 0.3f) * -stick_rx;

	//プレイヤーの向きを回転
	rotation_.y += rot_horizon;
}