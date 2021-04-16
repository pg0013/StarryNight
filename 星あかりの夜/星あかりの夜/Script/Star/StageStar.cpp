/**
 * @file    Star.cpp
 * @brief  ステージ内のスター挙動実装クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/01/23
 */

#include "StageStar.h"
#include"StageStarWaitState.h"
#include"StageStarFollowState.h"
#include"StageStarDiffusionState.h"
#include"../Player/Player.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::star;

Star::Star()
{
	status_ = STATUS::WAIT;
	rot_speed_ = 5.0f;
	jump_height_ = 4.5f;
	jump_speed_ = -jump_height_;
	star_num_ = 0;

	gravity_ = 0.1f;
	ground_position_y_ = 0.0f;

	//初期は待機状態に初期化
	stagestar_state_ = std::make_shared<StageStarWaitState>();
	state_map_.clear();
}

Star::~Star()
{
	state_map_.clear();
}

void Star::Initialize()
{
	position_ = MV1GetPosition(handle_);
	rotation_ = MV1GetRotationXYZ(handle_);
	ground_position_y_ = position_.y;

	//状態ポインタを管理マップに追加
	state_map_.emplace("Wait", stagestar_state_);
	state_map_.emplace("Follow", std::make_shared<StageStarFollowState>());
	state_map_.emplace("Diffusion", std::make_shared<StageStarDiffusionState>());
}

void Star::Process()
{
	//状態遷移の切り替え
	stagestar_state_->Input(*this);
	//状態の更新処理
	stagestar_state_->Update(*this);
}

void Star::ChangeStageStarState(const std::string& _state_name)
{
	auto iter = state_map_.find(_state_name);

	if (iter != state_map_.end())
	{
		//現在の状態の出口処理を実行
		stagestar_state_->Exit(*this);

		//指定した状態に切り替え
		stagestar_state_ = iter->second;

		//切り替えた状態の入口処理を実行
		stagestar_state_->Enter(*this);
	}
}

int Star::IsStageStarDraw()
{
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	bool star_draw_flag = true;

	//プレイヤーが射撃状態かどうか取得
	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			std::shared_ptr<player::Player> player = std::dynamic_pointer_cast<player::Player>(iter);
			star_draw_flag = player->GetPlayerSlingShotStatus();
			break;
		}
	}

	if (star_draw_flag)
		return FALSE;
	else
		return TRUE;
}

void Star::Render()
{
	MV1SetPosition(handle_, position_);
	MV1SetRotationXYZ(handle_, rotation_);

	MV1SetVisible(handle_, IsStageStarDraw());
	MV1DrawModel(handle_);

#ifdef DEBUG_FUNCTION
	DrawLine3D(position_, VAdd(position_, VGet(0, 2000, 0)), GetColor(255, 255, 0));
#endif // debug_
}