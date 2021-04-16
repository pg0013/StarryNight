/**
 * @file    PlayerShootState.cpp
 * @brief  プレイヤーの射撃状態処理
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include "PlayerShootState.h"
#include"Player.h"
#include"../Mode/ModeGame.h"
#include"../Mode/ModeOverlay.h"

using namespace starrynight::player;

PlayerShootState::PlayerShootState()
{
	shoot_charge_effect_flag_ = false;
}

PlayerShootState::~PlayerShootState()
{
}

void PlayerShootState::Enter(Player& _player)
{
	_player.SetPlayerSlingShotStatus(true);

	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	//ゲーム内UIを非表示にする
	mode_game->ui_.SetDrawPlayerUIFlag(false);
	//射撃標準を描画
	mode_game->ui_.shoot_ui_.SetDrawShootGuide(true);
}

void PlayerShootState::Exit(Player& _player)
{
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	//射撃状態を解除
	_player.SetPlayerSlingShotStatus(false);

	//UIを表示
	mode_game->ui_.SetDrawPlayerUIFlag(true);
	//射撃ガイドを非表示
	mode_game->ui_.shoot_ui_.SetDrawShootGuide(false);

	//フラグを初期化
	_player.SetSelectedStarFlag(false);
	shoot_charge_effect_flag_ = false;
}

void PlayerShootState::Input(Player& _player)
{
	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	//星選択前であれば、トリガーを離すと射撃状態を解除
	if (x_input.RightTrigger != 255 &&
		_player.GetSelectedStarFlag() == false)
	{
		_player.ChangePlayerState("Wait");
		return;
	}

	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	if (_player.GetPlayerStatus() == Player::STATUS::SHOOT_END &&
		_player.GetAnimPlayTime() > _player.GetAnimTotalTime() &&
		mode_game->IsClearStage() == false)
	{
		//残り星数を点滅表示
		mode_game->ui_.score_ui_.SetBlinkingFlag(true);

		//ステージのスター総数とプレイヤーの所持スター数を設定
		mode_game->AddStageStarNum(-1 * mode_game->GetPlayerStarNum());
		mode_game->SetPlayerStarNum(0);

		_player.ChangePlayerState("Wait");
		return;
	}
}

void PlayerShootState::Update(Player& _player)
{
	HoldSlingShot(_player);
}