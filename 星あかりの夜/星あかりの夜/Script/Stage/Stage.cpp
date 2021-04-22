/**
 * @file    Stage.cpp
 * @brief  ステージ管理クラス
 *
 * @author Takuya Fujisawa
 * @date   2020/12/17
 */
#include "Stage.h"
#include"../Mode/ModeGame.h"
#include<algorithm>

using namespace starrynight::stage;

Stage* Stage::instance_ = nullptr;

Stage::Stage()
{
	instance_ = this;
	stage_handle_.clear();
	navimesh_handle_.clear();
	map_floortype_.clear();
}

Stage::~Stage()
{
	ClearHandle();
	instance_ = nullptr;
}

void Stage::Initialize(const std::string& _stage_name)
{
	stage_param_.LoadStage(_stage_name, false);
	star_param_.LoadStageStar(_stage_name, false);
	skystar_param_.LoadSkyStar(_stage_name, false);
	enemy_param_.LoadStageEnemys(_stage_name, false);

	InitStageModel();
	InitStageStar();
	InitSkyStar();
	InitEnemy();
	InitStageEffect();
}

void Stage::Process()
{
}

void Stage::Render()
{
	SetWriteZBuffer3D(FALSE);
	MV1DrawModel(skysphere_);
	SetWriteZBuffer3D(TRUE);

	//透過素材のあるオブジェクトを格納するコンテナ
	std::vector<handle> trans_object;

	for (auto iter : stage_handle_)
	{
		//透過素材のあるオブジェクトは後に描画する
		if (MV1GetSemiTransState(iter) == TRUE)
		{
			trans_object.push_back(iter);
			continue;
		}
		MV1DrawModel(iter);
	}

	//αテストを行い、透過テクスチャのあるオブジェクトを描画
	for (auto iter : trans_object)
	{
		MV1SetMaterialDrawAlphaTestAll(iter, TRUE, DX_CMP_GREATER, 51);
		MV1DrawModel(iter);
	}
}

void Stage::ClearHandle()
{
	stage_handle_.clear();

	for (auto iter : navimesh_handle_)
	{
		MV1TerminateCollInfo(iter, MV1SearchFrame(iter, "floor_NavMesh"));
		MV1TerminateCollInfo(iter, MV1SearchFrame(iter, "wall_NavMesh"));
	}
	navimesh_handle_.clear();
	map_floortype_.clear();
}

int Stage::CheckFloorType(std::string& _filename)
{
	//ファイル名を小文字に変更
	std::transform(_filename.begin(), _filename.end(), _filename.begin(), tolower);

	if (std::equal(_filename.begin(), _filename.begin() + 3, "rock"))
	{
		return ROCK;
	}
	else if (std::equal(_filename.begin(), _filename.begin() + 4, "stone") ||
		std::equal(_filename.begin(), _filename.begin() + 5, "stairs") ||
		std::equal(_filename.begin(), _filename.begin() + 4, "shoot"))
	{
		return ROCK;
	}
	else if (std::equal(_filename.begin(), _filename.begin() + 5, "bridge"))
	{
		return WOOD;
	}
	else
	{
		return GRASS;
	}
}