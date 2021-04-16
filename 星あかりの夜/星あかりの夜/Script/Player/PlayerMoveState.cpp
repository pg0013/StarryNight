/**
 * @file    PlayerMoveState.cpp
 * @brief  プレイヤーの移動状態処理
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include "PlayerMoveState.h"
#include"Player.h"
#include"../Stage/Stage.h"
#include"../Camera/Camera.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::player;

PlayerMoveState::PlayerMoveState(Player& _player)
{
	walk_speed_ = _player.player_param_.GetPlayerParam("walk_speed");
	run_speed_ = _player.player_param_.GetPlayerParam("run_speed");
	rot_speed_ = _player.player_param_.GetPlayerParam("rot_speed");
	move_ = { 0,0,0 };
	position_ = { 0,0,0 };
	old_position_ = { 0,0,0 };
}

PlayerMoveState::~PlayerMoveState()
{
}

void PlayerMoveState::Enter(Player& _player)
{
}

void PlayerMoveState::Exit(Player& _player)
{
}

void PlayerMoveState::Input(Player& _player)
{
	//ダメージ判定があれば、ダメージ状態へ移行
	if (_player.GetDamageFlag() == true)
	{
		_player.ChangePlayerState("Damage");
		return;
	}

	//腰から地面までの線分ベクトル
	VECTOR start_line = VAdd(_player.GetPosition(), VGet(0, 40.0f, 0));
	VECTOR end_line = VAdd(_player.GetPosition(), VGet(0, -5.0f, 0));

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;
	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

	//崖などから飛び降りたら落下状態にして、ジャンプ中に設定する
	if (!hit_poly_floor.HitFlag)
	{
		_player.ChangePlayerState("Jump");
		return;
	}

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	//接地状態でBボタンが入力されたら、ジャンプ状態へ移行
	if (trigger_key & PAD_INPUT_2 &&
		hit_poly_floor.HitFlag)
	{
		_player.ChangePlayerState("Jump");
		return;
	}

	//スティック入力がなければ、待機状態へ移行
	if (utility::GetLeftStickLength() < ANALOG_MIN)
	{
		_player.ChangePlayerState("Wait");
		return;
	}

	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_shootpoint;

	//腰から地面までの線分ベクトル
	start_line = VAdd(_player.GetPosition(), VGet(0, 40.0f, 0));
	end_line = VAdd(_player.GetPosition(), VGet(0, -30.0f, 0));

	//射撃ポイントとの当たり判定
	hit_poly_shootpoint = stage::Stage::GetInstance()->GetHitLineToShootPoint(start_line, end_line);

	//射撃地点にいれば次へ
	if (!hit_poly_shootpoint.HitFlag)
		return;

	if (x_input.RightTrigger == 255 &&//Rトリガー押し込み
		mode_game->GetPlayerStarNum() > 0)//スターを所持している
	{
		_player.ChangePlayerState("Shoot");
		return;
	}
}

void PlayerMoveState::Update(Player& _player)
{
	old_position_ = _player.GetPosition();
	position_ = old_position_;

	//移動処理
	Move(_player);
	//壁ずり処理
	SlidingWall(_player);
	//プレイヤーの向きを決定
	DecideForwardDirection(_player);

	//歩きか走りかを切り替え
	SwitchMoveStatus(_player);

	_player.SetPosition(position_);
	//移動分カメラを追従させる
	camera::Camera::GetInstance()->PlayerFollowMove(position_, old_position_);
}

void PlayerMoveState::SwitchMoveStatus(Player& _player)
{
	float run_stick_range = 0.6f;

	//スティックの角度で歩きか走りかを設定
	if (utility::GetLeftStickLength() < run_stick_range)
	{
		_player.SetPlayerStatus(Player::STATUS::WALK);
	}
	else
	{
		_player.SetPlayerStatus(Player::STATUS::RUN);
	}
}