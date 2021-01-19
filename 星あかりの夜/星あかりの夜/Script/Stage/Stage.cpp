/**
 * @file    Stage.cpp
 * @brief  ステージ管理クラス
 *
 * @author Takuya Fujisawa
 * @date   2020/12/17
 */
#include "Stage.h"

using namespace starrynight::stage;

Stage* Stage::instance_ = nullptr;

Stage::Stage()
{
	instance_ = this;
	stage_handle_.clear();
	navimesh_handle_.clear();
}

Stage::~Stage()
{
	ClearHandle();
	instance_ = nullptr;
}

void Stage::Initialize()
{
	stage_param_.LoadStage("field_B", false);

	auto handle_map = stage_param_.GetMapModelParam();
	for (auto iter = handle_map.begin(); iter != handle_map.end(); iter++)
	{
		auto handle = resource::ResourceServer::GetModelHandle((*iter).second.handlename_);
		stage_handle_.push_back(handle);

		if (MV1SearchFrame(handle, "field_B_NavMesh_GEO") > 0)
		{
			MV1SetupCollInfo(handle, MV1SearchFrame(handle, "field_B_NavMesh_GEO"), 16, 16, 16);
			//MV1SetFrameVisible(handle, MV1SearchFrame(handle, "field_B_NavMesh_GEO"), FALSE);
			navimesh_handle_.push_back(handle);
		}
		if (MV1SearchFrame(handle, "foothold_NavMesh") > 0)
		{
			MV1SetupCollInfo(handle, MV1SearchFrame(handle, "foothold_NavMesh"), 16, 16, 16);
			navimesh_handle_.push_back(handle);
		}
		if (MV1SearchFrame(handle, "step_NavMesh") > 0)
		{
			MV1SetupCollInfo(handle, MV1SearchFrame(handle, "step_NavMesh"), 16, 16, 16);
			navimesh_handle_.push_back(handle);
		}
	}
}

void Stage::Process()
{
}

void Stage::Render()
{
	std::vector<handle> trans_object;
	for (auto iter : stage_handle_)
	{
		if (MV1GetSemiTransState(iter) == TRUE)
		{
			trans_object.push_back(iter);
			continue;
		}
		MV1DrawModel(iter);
	}
	for (auto iter : trans_object)
	{
		MV1SetMaterialDrawAlphaTestAll(iter, TRUE, DX_CMP_GREATER, 51);
		MV1DrawModel(iter);
	}
}

void Stage::ClearHandle()
{
	for (auto iter : stage_handle_)
	{
		MV1DeleteModel(iter);
	}
	stage_handle_.clear();
}

