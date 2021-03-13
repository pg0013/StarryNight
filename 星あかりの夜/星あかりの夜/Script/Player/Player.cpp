/**
 * @file    Player.cpp
 * @brief  ゲームモード内のプレイヤークラス
 *
 * @author Takuya Fujisawa
 * @date    202012/16
 */
#include "Player.h"
#include"../Mode/ModeGame.h"
#include"../Effect/PlayerTrailEffect.h"
#include<vector>
using namespace starrynight::player;

Player::Player(std::string _stage_name)
{
	player_param_.LoadModelParameters("player");

	std::vector<std::string> param = { "walk_speed","run_speed","rot_speed" };
	player_param_.LoadPlayerParameters(param);
	handle_ = resource::ResourceServer::GetModelHandle("player");

	if (std::equal(_stage_name.begin(), _stage_name.end(), "haru_A"))
	{
		SetPosition(VGet(-607, 0, -2048));
	}
	else if (std::equal(_stage_name.begin(), _stage_name.end(), "haru_B"))
	{
		SetPosition(VGet(-85, 0, 2200));
		SetRotation(VGet(0, DEG2RAD(180.0f), 0));
	}
	else if (std::equal(_stage_name.begin(), _stage_name.end(), "haru_C"))
	{
		SetPosition(VGet(823, 0, -2143));
		SetRotation(VGet(0, DEG2RAD(-13.65f), 0));
	}

	walk_speed_ = player_param_.GetPlayerParam("walk_speed");
	run_speed_ = player_param_.GetPlayerParam("run_speed");
	rot_speed_ = player_param_.GetPlayerParam("rot_speed");

	player_hp_ = 3;

	jump_speed_ = 0.0f;
	jump_height_ = 15.0f;
	gravity_ = 0.5f;
	jump_flag_ = false;
	floor_type_ = -1;

	damage_flag_ = false;
	damage_anim_flag_ = false;

	gameover_flag_ = false;

	status_ = STATUS::NONE;
	anim_attach_index_ = -1;
	old_anim_attach_index_ = anim_attach_index_;
	anim_rate_ = 1.0f;
	anim_loop_flag_ = true;

	slingshot_flag_ = false;
	selected_skystar_flag_ = false;
	shoot_charge_effect_flag_ = true;

	Initialize();
}

Player::~Player()
{
}

void Player::Initialize()
{
	ObjectBase::Initialize();

	effect::PlayerTrailEffect* trail_effect = NEW effect::PlayerTrailEffect();
	trail_effect->PlayEffect();
	trail_effect->SetPosition(position_);
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	mode_game->effect_server_.Add(trail_effect);
}

void Player::Process()
{
	STATUS old_status = status_;

	SwitchPlayerAction();

	OutOfStage();
	GameOver();

	SwitchPlayerAnimation(old_status);
	SwitchPlayerSound();

	PlayerAnimationBlend();
}

void Player::Damage()
{
	if (damage_anim_flag_ == false)
		return;

	if (status_ != STATUS::DAMAGE)
		anim_play_time_ = 0;

	status_ = STATUS::DAMAGE;

	if (jump_flag_)
	{
		position_.y += jump_speed_;

		//腰から地面までの線分ベクトル
		VECTOR start_line = VAdd(position_, VGet(0, 40.0f, 0));
		VECTOR end_line = VAdd(position_, VGet(0, -5.0f, 0));

		//Navimeshとの当たり判定
		MV1_COLL_RESULT_POLY hit_poly_floor;
		hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

		//ジャンプ開始処理
		if (hit_poly_floor.HitFlag)
		{
			position_.y = hit_poly_floor.HitPosition.y;
			jump_flag_ = false;
		}
		//ジャンプ加速処理
		jump_speed_ -= gravity_;
	}

	//所持しているスターの数をリセットする
	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	mode_game->SetPlayerStarNum(0);

	//ダメージモーションが終了したら、待機へ移行
	if (anim_play_time_ >= anim_total_time_)
	{
		damage_flag_ = false;
		damage_anim_flag_ = false;
		status_ = STATUS::WAIT;
	}
}

void Player::OutOfStage()
{
	if (position_.y < -200.0f)
	{
		//ステージの際に配置
		float angle = atan2(position_.z, position_.x);
		float outstage_length = 2450.0f;
		position_ = VGet(outstage_length * cosf(angle), 0, outstage_length * sinf(angle));

		damage_flag_ = true;
		damage_anim_flag_ = true;
		anim_play_time_ = 0;

		VECTOR camera_position = camera::Camera::GetInstance()->GetPosition();
		camera::Camera::GetInstance()->SetPosition(VGet(camera_position.x, 100, camera_position.z));
		camera::Camera::GetInstance()->SetTarget(position_);
	}
}

void Player::GameOver()
{
	if (gameover_flag_)
		return;

	if (GetPlayerHP() > 0)
		return;

	gameover_flag_ = true;

	//ゲームオーバーへ移行
	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	mode_game->SetNextMode(300, 60, GAME_OVER);
}

void Player::PlayerAnimationBlend()
{
	//アニメーションブレンド処理
	if (anim_rate_ >= 1.0f)
		MV1SetAttachAnimTime(handle_, anim_attach_index_, anim_play_time_);
	else
	{
		MV1SetAttachAnimBlendRate(handle_, old_anim_attach_index_, 1.0f - anim_rate_);
		MV1SetAttachAnimBlendRate(handle_, anim_attach_index_, anim_rate_);
	}
}

void Player::Render()
{
	MV1SetPosition(handle_, position_);
	MV1SetRotationXYZ(handle_, rotation_);
	MV1DrawModel(handle_);

#ifdef DEBUG_FUNCTION
	//プレイヤーのカプセル情報
	VECTOR capsule_positon1 = VAdd(position_, VGet(0, 100, 0));
	VECTOR capsule_positon2 = VAdd(position_, VGet(0, 45, 0));
	float radius = 35.0f;
	DrawCapsule3D(capsule_positon1, capsule_positon2, radius, 16, DEBUG_COLOR, DEBUG_COLOR, FALSE);

	if (!jump_flag_)
		DrawLine3D(VAdd(position_, VGet(0, 40.0f, 0)), VAdd(position_, VGet(0, -10.0f, 0)), DEBUG_COLOR);

	VECTOR start = VAdd(position_, VGet(0, 100.0f, 0));
	VECTOR end = VAdd(start, VScale(VNorm(utility::GetForwardVector(rotation_.y)), 30.0f));
	DrawLine3D(start, end, DEBUG_COLOR);

	int x, y;
	x = 0; y = 12 * DEBUG_FONT_SIZE;
	switch (status_)
	{
	case starrynight::player::Player::STATUS::NONE:
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- status    : NONE", y / DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
		break;
	case starrynight::player::Player::STATUS::WAIT:
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- status    : WAIT", y / DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
		break;
	case starrynight::player::Player::STATUS::WALK:
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- status    : WALK", y / DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
		break;
	case starrynight::player::Player::STATUS::RUN:
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- status    : RUN", y / DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
		break;
	case starrynight::player::Player::STATUS::JUMP_START:
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- status    : JUMP_START", y / DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
		break;
	case starrynight::player::Player::STATUS::JUMP_LOOP:
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- status    : JUMP_LOOP", y / DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
		break;
	case starrynight::player::Player::STATUS::JUMP_END:
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- status    : JUMP_END", y / DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
		break;
	case starrynight::player::Player::STATUS::SHOOT_START:
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- status    : SHOOT_START", y / DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
		break;
	case starrynight::player::Player::STATUS::SHOOT_END:
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- status    : SHOOT_END", y / DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
		break;
	case starrynight::player::Player::STATUS::_EOT_:
		DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- status    : _EOT_", y / DEBUG_FONT_SIZE); y += DEBUG_FONT_SIZE;
		break;
	}

#endif
}