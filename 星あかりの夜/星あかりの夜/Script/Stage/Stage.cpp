/**
 * @file    Stage.cpp
 * @brief  ステージ管理クラス
 *
 * @author Takuya Fujisawa
 * @date   2020/12/17
 */
#include "Stage.h"

using namespace starrynight::stage;

Stage::Stage()
{
	stage_handle_.clear();
	field_ = 0;
	navimesh_ = 0;
}

Stage::~Stage()
{
	ClearHandle();
}

void Stage::Initialize()
{
	stage_param_.LoadStage("test", false);

	auto handle_map = stage_param_.GetMapModelParam();
	for (auto iter = handle_map.begin(); iter != handle_map.end(); iter++)
	{
		auto handle = resource::ResourceServer::GetModelHandle((*iter).second.handlename_);
		stage_handle_.push_back(handle);
	}

	field_ = resource::ResourceServer::GetModelHandle("field_B_GEO");
	MV1SetupCollInfo(field_, MV1SearchFrame(field_, "field_B_NaviMesh_GEO"), 16, 16, 16);
	MV1SetFrameVisible(field_, MV1SearchFrame(field_, "field_B_NaviMesh_GEO"), FALSE);
}

void Stage::Process()
{

}

void Stage::Render()
{
	for (auto iter : stage_handle_)
	{
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