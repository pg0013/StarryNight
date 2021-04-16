/**
 * @file    HoldSlingShot.cpp
 * @brief  パチンコを構える処理
 *
 * @author Takuya Fujisawa
 * @date    202101/18
 */

#include"PlayerShootState.h"
#include"Player.h"
#include"../Camera/Camera.h"
#include"../Star/StageStar.h"
#include"../Star/SkyStar.h"
#include"../Effect/ShootChargeEffect.h"
#include"../Effect/ShootEffect.h"
#include"../Mode/ModeGame.h"
#include"../Mode/ModeOverlay.h"

using namespace starrynight::player;

void PlayerShootState::HoldSlingShot(Player& _player)
{
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	if (_player.GetPlayerStatus() != Player::STATUS::SHOOT_END)
	{
		//射撃構え処理
		SlingShotStance(_player);
		//星を選べ画像を描画
		mode_game->ui_.shoot_ui_.SetDrawChooseSkystarGuide(true);
	}

	//星との当たり判定を取得
	MV1_COLL_RESULT_POLY hit_star = CheckHitStar(_player);

	//星と標準があっていないので、次の処理に進まない
	if (!hit_star.HitFlag)
		return;

	//星と当たり判定があれば、星を選べ画像を表示しない
	mode_game->ui_.shoot_ui_.SetDrawChooseSkystarGuide(false);

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	//Yボタンが押されたら、タイミングゲージを描画
	if (trigger_key & PAD_INPUT_4 &&
		_player.GetSelectedStarFlag() == false)
	{
		mode_game->ui_.timing_ui_.SetSelectedStarNum(hit_star.FrameIndex);
		mode_game->ui_.shoot_ui_.SetDrawShootGuide(false);
		mode_game->ui_.timing_ui_.SetDrawTimingGuide(true);
		mode_game->ui_.timing_ui_.SetSelectStarFrame(mode_game->GetModeCount());
		_player.SetSelectedStarFlag(true);

		_player.se_.Load("Resource/sound/star_select.wav");
		_player.se_.Play();
		_player.se_.Fade(0.0f, 1.0f);
	}

	//タイミングゲームが終了し、星が発射可能であれば星を撃つ
	if (mode_game->ui_.timing_ui_.GetLaunchStarShoot() == true)
	{
		Launch_Star(_player, hit_star.HitPosition);
	}

	if (mode_game->IsClearStage() == false)
		return;

	//星発射後にゲームクリアしていれば、星座カメラに移行するのでフェードアウト
	if (_player.GetAnimPlayTime() == 70.0f)
	{
		std::shared_ptr<mode::ModeOverlay> modeoverlay = std::make_shared < mode::ModeOverlay >();
		modeoverlay->Fade(11, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(modeoverlay, 0, "Overlay");
	}

	if (_player.GetAnimPlayTime() == _player.GetAnimTotalTime())
	{
		mode_game->CheckIsClearStage();
	}
}

void  PlayerShootState::SlingShotStance(Player& _player)
{
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	_player.SetPlayerStatus(Player::STATUS::SHOOT_START);

	//射撃溜めエフェクト生成
	if (_player.GetAnimPlayTime() > _player.GetAnimTotalTime())
	{
		//1フレームだけ処理を行う
		if (shoot_charge_effect_flag_ == false)
		{
			std::shared_ptr<effect::ShootChargeEffect> charge_effect = std::make_shared<effect::ShootChargeEffect>();

			charge_effect->Initialize();
			charge_effect->PlayEffect();
			mode_game->effect_server_.Add(charge_effect);
			shoot_charge_effect_flag_ = true;
		}
	}

	//射撃標準の回転を行う
	if (_player.GetSelectedStarFlag() == false)
	{
		SetShootRotation(_player);
	}
}

MV1_COLL_RESULT_POLY PlayerShootState::CheckHitStar(Player& _player)
{
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	MV1_COLL_RESULT_POLY hit_star;
	hit_star.HitFlag = 0;

	//射撃標準の地点への線分
	VECTOR start = camera::Camera::GetInstance()->GetPosition();
	VECTOR end = ConvScreenPosToWorldPos(
		VGet(appframe::SCREEN_WIDTH / 2.0f, appframe::SCREEN_HEIGHT / 2.0f, 1.0f));

	//星座との当たり判定をとる
	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::SKY_STAR)
		{
			std::shared_ptr<star::SkyStar> star = std::dynamic_pointer_cast<star::SkyStar>(iter);
			hit_star = star->GetHitToSkyStar(start, end);
			break;
		}
	}

	return hit_star;
}

void PlayerShootState::Launch_Star(Player& _player, const VECTOR& _star_position)
{
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	//射撃エフェクトの生成
	std::shared_ptr< effect::ShootEffect> shoot_effect = std::make_shared< effect::ShootEffect>(_star_position);

	shoot_effect->PlayEffect();
	shoot_effect->Initialize();
	mode_game->effect_server_.Add(shoot_effect);

	//プレイヤーが連れていた星を削除する
	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::STAGE_STAR)
		{
			std::shared_ptr<star::Star> star = std::dynamic_pointer_cast<star::Star>(iter);

			if (star->GetPlayersStarNum() > 0)
				mode_game->object_server_.Delete(iter);
		}
	}

	_player.SetPlayerStatus(Player::STATUS::SHOOT_END);

	_player.se_.Load("Resource/sound/shoot_voice.wav");
	_player.se_.Play();

	mode_game->ui_.timing_ui_.SetLaunchStarShoot(false);
}

void PlayerShootState::SetShootRotation(Player& _player)
{
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	VECTOR rotation = _player.GetRotation();

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
	rotation.y += rot_horizon;
	_player.SetRotation(rotation);
}