/**
 * @file    PlayerWaitState.cpp
 * @brief  プレイヤーの待機状態処理
 *
 * @author Takuya Fujisawa
 * @date   2021/04/15
 */

#include "PlayerWaitState.h"
#include"Player.h"
#include"../Stage/Stage.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::player;

PlayerWaitState::PlayerWaitState()
{
}

PlayerWaitState::~PlayerWaitState()
{
}

void PlayerWaitState::Enter(Player& _player)
{
	_player.SetPlayerStatus(Player::STATUS::WAIT);
}

void PlayerWaitState::Exit(Player& _player)
{
}

void PlayerWaitState::Input(Player& _player)
{
	//スティック入力があれば移動状態へ移行
	if (utility::GetLeftStickLength() > ANALOG_MIN)
	{
		_player.ChangePlayerState("Move");
		return;
	}

	//ダメージ判定があれば、ダメージ状態へ移行
	if (_player.GetDamageFlag() == true)
	{
		_player.ChangePlayerState("Damage");
		return;
	}

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	//腰から地面までの線分ベクトル
	VECTOR start_line = VAdd(_player.GetPosition(), VGet(0, 40.0f, 0));
	VECTOR end_line = VAdd(_player.GetPosition(), VGet(0, -5.0f, 0));

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;
	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

	//接地状態でBボタンが入力されたら、ジャンプ状態へ移行
	if (trigger_key & PAD_INPUT_2 &&
		hit_poly_floor.HitFlag)
	{
		_player.ChangePlayerState("Jump");
		return;
	}

	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();

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

void PlayerWaitState::Update(Player& _player)
{
}