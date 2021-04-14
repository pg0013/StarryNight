/**
 * @file    StageStarWaitState.cpp
 * @brief  フィールド内スターの待機状態クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include "StageStarWaitState.h"
#include"StageStar.h"

using namespace starrynight::star;

StageStarWaitState::StageStarWaitState()
{
	touch_length_ = 100.0f;
	detect_length_ = 1000.0f;
}

StageStarWaitState::~StageStarWaitState()
{
}

void StageStarWaitState::Enter(Star& _star)
{
}

void StageStarWaitState::Exit(Star& _star)
{
}

void StageStarWaitState::Input(Star& _star)
{
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR distance = VSub(VAdd(player_position, VGet(0, 80, 0)), VAdd(_star.GetPosition(), VGet(0, 40, 0)));

	float length = VSize(distance);

	//取得範囲内であれば、追跡状態へ移行
	if (length < touch_length_)
	{
		_star.ChangeStageStarState("Follow");
	}
}

void StageStarWaitState::Update(Star& _star)
{
	Wait(_star);
}