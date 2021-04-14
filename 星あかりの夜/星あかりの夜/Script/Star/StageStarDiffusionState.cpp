/**
 * @file    StageStarDiffusionState.cpp
 * @brief  フィールド内スターの拡散状態クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include "StageStarDiffusionState.h"
#include"../Stage/Stage.h"
#include"StageStar.h"

using namespace starrynight::star;

StageStarDiffusionState::StageStarDiffusionState()
{
}

StageStarDiffusionState::~StageStarDiffusionState()
{
}

void StageStarDiffusionState::Enter(Star& _star)
{
	_star.SetJumpSpeed(_star.GetJumpHeight() * 3.0f);

}

void StageStarDiffusionState::Exit(Star& _star)
{
	_star.SetGroundPositionY(_star.GetPosition().y);
}

void StageStarDiffusionState::Input(Star& _star)
{
	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;

	//地面までの線分ベクトル
	VECTOR start_line = _star.GetPosition();
	VECTOR end_line = VAdd(_star.GetPosition(), VGet(0, -50.0f, 0));

	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

	//着地したら待機状態へ移行
	if (hit_poly_floor.HitFlag)
	{
		_star.ChangeStageStarState("Wait");
	}
}

void StageStarDiffusionState::Update(Star& _star)
{
	Diffusion(_star);
}