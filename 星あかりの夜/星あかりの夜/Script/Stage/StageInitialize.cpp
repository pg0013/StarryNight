/**
 * @file    StageInitialize.cpp
 * @brief  ステージオブジェクトの初期化処理
 *
 * @author Takuya Fujisawa
 * @date   2021/03/13
 */

#include "Stage.h"
#include"../Star/Star.h"
#include"../Star/SkyStar.h"
#include"../Enemy/Enemy.h"
#include"../Effect/ShootPointEffect.h"
#include"../Effect/StageFlowEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::stage;

void Stage::InitStageModel()
{
	//Stageモデルの読み込み
	auto handle_map = stage_param_.GetMapModelParam();

	for (auto&& iter : handle_map)
	{
		auto navmesh_count = 0;//ナビメッシュがあるオブジェクトかを確認するカウンタ
		auto handle = resource::ResourceServer::GetModelHandle(iter.second.handlename_);

		//天球は初めに描画するために個別で管理する
		if (handle == resource::ResourceServer::GetModelHandle("skysphere"))
		{
			skysphere_ = handle;
			continue;
		}

		stage_handle_.push_back(handle);

		//射撃ポイントは当たり判定のために個別で管理する
		if (handle == resource::ResourceServer::GetModelHandle("ShootPoint_GEO1"))
		{
			shootpoint_ = handle;
		}

		//壁や床の当たり判定があるオブジェクトは当たり判定用のコンテナにも格納する
		if (MV1SearchFrame(handle, "floor_NavMesh") > 0)
		{
			MV1SetupCollInfo(handle, MV1SearchFrame(handle, "floor_NavMesh"), 16, 16, 16);
			MV1SetFrameVisible(handle, MV1SearchFrame(handle, "floor_NavMesh"), FALSE);

			map_floortype_.emplace(handle, CheckFloorType(iter.second.filename_));

			navmesh_count++;
		}
		if (MV1SearchFrame(handle, "wall_NavMesh") > 0)
		{
			MV1SetupCollInfo(handle, MV1SearchFrame(handle, "wall_NavMesh"), 16, 16, 16);
			MV1SetFrameVisible(handle, MV1SearchFrame(handle, "wall_NavMesh"), FALSE);
			navmesh_count++;
		}

		if (navmesh_count > 0)
			navimesh_handle_.push_back(handle);
	}
}

void Stage::InitStageStar()
{
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();

	//Starモデルの配置情報を読み込み
	auto handle_map = star_param_.GetMapModelParam();

	//StageにStarモデルを配置する
	int stage_star_num = 0;
	for (auto&& iter : handle_map)
	{
		star::Star* star = NEW star::Star();
		star->SetModelHandle(resource::ResourceServer::GetModelHandle(iter.second.handlename_));
		star->Initialize();
		mode_game->object_server_.Add(star);
		stage_star_num++;
	}

	//ステージのフィールドスター総数を設定
	mode_game->SetStageStarNum(stage_star_num);
	mode_game->SetStageStarNumAll(stage_star_num);
}

void Stage::InitSkyStar()
{
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();

	//星座の情報を読み込み
	auto handle_map = skystar_param_.GetMapModelParam();

	//星座モデルを配置する
	for (auto&& iter : handle_map)
	{
		star::SkyStar* skystar = NEW star::SkyStar(iter.second.filename_);
		skystar->Initialize();
		mode_game->object_server_.Add(skystar);
	}
}

void Stage::InitEnemy()
{
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();

	//エネミーモデルの配置情報を読み込み
	auto handle_map = enemy_param_.GetMapModelParam();

	//Stageにエネミーモデルを配置する
	for (auto&& iter : handle_map)
	{
		enemy::Enemy* enemy = NEW enemy::Enemy(iter.second.handlename_);
		enemy->Initialize();
		mode_game->object_server_.Add(enemy);
	}
}

void Stage::InitStageEffect()
{
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();

	//射撃ポイントにエフェクトを配置
	handle shootpoint = resource::ResourceServer::GetModelHandle("ShootPoint_GEO1");
	effect::ShootPointEffect* shootpoint_effect = NEW effect::ShootPointEffect();
	shootpoint_effect->SetPosition(MV1GetPosition(shootpoint));
	shootpoint_effect->PlayEffect();
	mode_game->effect_server_.Add(shootpoint_effect);

	//花びらエフェクトの生成
	effect::StageFlowEffect* stageflow_effect = NEW effect::StageFlowEffect();
	stageflow_effect->SetPosition(VGet(0, 0, 0));
	stageflow_effect->PlayEffect();
	mode_game->effect_server_.Add(stageflow_effect);
}