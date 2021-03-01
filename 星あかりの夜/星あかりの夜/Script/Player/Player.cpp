/**
 * @file    Player.cpp
 * @brief  ゲームモード内のプレイヤークラス
 *
 * @author Takuya Fujisawa
 * @date    202012/16
 */
#include "Player.h"
#include"../Mode/ModeGame.h"
#include<vector>
using namespace starrynight::player;

Player::Player()
{
	player_param_.LoadModelParameters("player");
	std::vector<std::string> param = { "walk_speed","run_speed","rot_speed" };
	player_param_.LoadPlayerParameters(param);

	handle_ = resource::ResourceServer::GetModelHandle("player");

	walk_speed_ = player_param_.GetPlayerParam("walk_speed");
	run_speed_ = player_param_.GetPlayerParam("run_speed");
	rot_speed_ = player_param_.GetPlayerParam("rot_speed");

	jump_speed_ = 0.0f;
	gravity_ = 0.5f;
	jump_flag_ = false;

	damage_flag_ = false;
	damage_anim_flag_ = false;

	status_ = STATUS::NONE;
	anim_attach_index_ = -1;
	old_anim_attach_index_ = anim_attach_index_;
	anim_rate_ = 1.0f;
	anim_loop_flag_ = true;

	shoot_charge_effect_flag_ = true;

	hitposition = VGet(0, 0, 0);

	Initialize();
}

Player::~Player()
{
}

void Player::Initialize()
{
	ObjectBase::Initialize();
}

void Player::Process()
{
	STATUS old_status = status_;

	SwitchPlayerAction();

	OutOfStage();

	SwitchPlayerAnimation(old_status);
	PlayerAnimationBlend();
}

void Player::Damage()
{
	if (damage_anim_flag_ == false)
		return;

	status_ = STATUS::DAMAGE;

	//所持しているスターの数をリセットする
	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	mode_game->SetPlayerStarNum(0);

	if (anim_play_time_ >= anim_total_time_)
	{
		damage_flag_ = false;
		damage_anim_flag_ = false;
		status_ = STATUS::WAIT;
	}
}

void Player::OutOfStage()
{
	if (position_.y < -100)
	{
		position_ = VGet(0, 100, 0);
		camera::Camera::GetInstance()->SetPosition(VGet(0, 90.f, -300.f));
		camera::Camera::GetInstance()->SetTarget(VGet(0.0f, 60.0f, 0.0f));
	}
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

	//射撃標準の地点への線分
	end = hitposition;
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