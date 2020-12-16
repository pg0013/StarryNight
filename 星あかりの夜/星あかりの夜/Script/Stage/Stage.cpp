#include "Stage.h"

using namespace starrynight;

Stage::Stage()
{
	stage_handle_.clear();
}

Stage::~Stage()
{
	ClearHandle();
}

void Stage::Initialize()
{
	stage_param_.LoadStage("test", false);

	auto handle_map = stage_param_.GetMapModelParam();
	for (auto iter  = handle_map.begin();iter!= handle_map.end();iter++)
	{
		auto handle = resource::ResourceServer::GetModelHandle((*iter).second.handlename_);
		stage_handle_.push_back(handle);
	}
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