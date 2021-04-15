/**
 * @file    PlayerJumpState.cpp
 * @brief  プレイヤーのジャンプ状態処理
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include "PlayerJumpState.h"
#include"Player.h"
#include"../Stage/Stage.h"
#include"../Camera/Camera.h"

using namespace starrynight::player;

PlayerJumpState::PlayerJumpState(Player& _player) : PlayerMoveState(_player)
{
	jump_speed_ = 0.0f;
	jump_height_ = 15.0f;
	gravity_ = 0.5f;
}

PlayerJumpState::~PlayerJumpState()
{
}

void PlayerJumpState::Enter(Player& _player)
{
	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	//腰から地面までの線分ベクトル
	VECTOR start_line = VAdd(_player.GetPosition(), VGet(0, 40.0f, 0));
	VECTOR end_line = VAdd(_player.GetPosition(), VGet(0, -5.0f, 0));

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;
	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

	//地面からジャンプしたか、落下したかを評価しジャンプ速度を設定
	if (trigger_key & PAD_INPUT_2 &&
		hit_poly_floor.HitFlag)
	{
		jump_speed_ = jump_height_;
		_player.SetPlayerStatus(Player::STATUS::JUMP_START);
	}
	else
	{
		jump_speed_ = 0.0f;
		_player.SetPlayerStatus(Player::STATUS::JUMP_LOOP);
	}
}

void PlayerJumpState::Exit(Player& _player)
{
}

void PlayerJumpState::Input(Player& _player)
{
	//ダメージ判定があれば、ダメージ状態へ移行
	if (_player.GetDamageFlag() == true)
	{
		_player.ChangePlayerState("Damage");
		return;
	}

	//着地後に歩き状態へ移行
	if (_player.GetPlayerStatus() == Player::STATUS::JUMP_END &&
		_player.GetAnimPlayTime() > _player.GetAnimTotalTime())
	{
		_player.ChangePlayerState("Move");
		return;
	}
}

void PlayerJumpState::Update(Player& _player)
{
	old_position_ = _player.GetPosition();
	position_ = old_position_;

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	//地面からジャンプしたか、落下したかを評価しジャンプ速度を設定
	if (trigger_key & PAD_INPUT_2 &&
		_player.GetPlayerStatus() == Player::STATUS::JUMP_END &&
		_player.GetAnimPlayTime() > _player.GetAnimTotalTime() / 2.0f)
	{
		jump_speed_ = jump_height_;
		_player.SetPlayerStatus(Player::STATUS::JUMP_START);
	}

	Move(_player);
	Jump(_player);
	SlidingWall(_player);
	SwitchJumpState(_player);
	Landing(_player);
	DecideForwardDirection(_player);

	_player.SetPosition(position_);
	//移動分カメラを追従させる
	camera::Camera::GetInstance()->PlayerFollowMove(position_, old_position_);
}

void PlayerJumpState::SwitchJumpState(Player& _player)
{
	//ジャンプアニメーションを設定
	if (jump_speed_ < 0.0f)
	{
		VECTOR start_line = VAdd(_player.GetPosition(), VGet(0, 40.0f, 0));
		VECTOR end_line = VAdd(_player.GetPosition(), VGet(0, -40.0f, 0));

		//Navimeshとの当たり判定
		MV1_COLL_RESULT_POLY hit_jump_floor;
		hit_jump_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

		if (hit_jump_floor.HitFlag)
		{
			_player.SetPlayerStatus(Player::STATUS::JUMP_END);

			if (_player.GetAnimPlayTime() != 0)
				return;

			//高い音から落ちたらボイスを再生する
			if (jump_speed_ < -20)
			{
				_player.se_.Load("Resource/sound/landing_voice.wav");
				_player.se_.Play();
			}
			else
			{
				_player.se_.Load("Resource/sound/shirimoti.wav");
				_player.se_.SetVolume_dB(-12.0f);
				_player.se_.Play();
			}
		}
		else
		{
			_player.SetPlayerStatus(Player::STATUS::JUMP_LOOP);
		}
	}
}