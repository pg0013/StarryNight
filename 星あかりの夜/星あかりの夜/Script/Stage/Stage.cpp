/**
 * @file    Stage.cpp
 * @brief  ステージ管理クラス
 *
 * @author Takuya Fujisawa
 * @date   2020/12/17
 */
#include "Stage.h"
#include"../Mode/ModeGame.h"
#include"../Star/Star.h"
#include"../Star/SkyStar.h"
#include"../Enemy/Enemy.h"
#include"../Effect/ShootPointEffect.h"
#include"../Effect/StageFlowEffect.h"

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

void Stage::Initialize(std::string _stage_name)
{


	stage_param_.LoadStage(_stage_name, false);
	star_param_.LoadStageStar(_stage_name, false);
	skystar_param_.LoadSkyStar(_stage_name, false);
	enemy_param_.LoadStageEnemys(_stage_name, false);

	//Stageモデルの読み込み
	auto handle_map = stage_param_.GetMapModelParam();
	for (auto iter = handle_map.begin(); iter != handle_map.end(); iter++)
	{
		auto navmesh_count = 0;//ナビメッシュがあるオブジェクトかを確認するカウンタ
		auto handle = resource::ResourceServer::GetModelHandle((*iter).second.handlename_);

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

	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	//Starモデルの配置情報を読み込み
	handle_map = star_param_.GetMapModelParam();

	//StageにStarモデルを配置する
	int stage_star_num = 0;
	for (auto iter = handle_map.begin(); iter != handle_map.end(); iter++)
	{
		star::Star* star = NEW star::Star();
		star->SetModelHandle(resource::ResourceServer::GetModelHandle((*iter).second.handlename_));
		star->Initialize();
		mode_game->object_server_.Add(star);
		stage_star_num++;
	}
	//ステージのフィールドスター総数を設定
	mode_game->SetStageStarNum(stage_star_num);

	//星座の情報を読み込み
	handle_map = skystar_param_.GetMapModelParam();

	//星座モデルを配置する
	for (auto iter = handle_map.begin(); iter != handle_map.end(); iter++)
	{
		star::SkyStar* skystar = NEW star::SkyStar((*iter).second.filename_);
		skystar->Initialize();
		mode_game->object_server_.Add(skystar);
	}
	//エネミーモデルの配置情報を読み込み
	handle_map = enemy_param_.GetMapModelParam();

	//Stageにエネミーモデルを配置する
	for (auto iter = handle_map.begin(); iter != handle_map.end(); iter++)
	{
		enemy::Enemy* enemy = NEW enemy::Enemy((*iter).second.handlename_);
		enemy->Initialize();
		mode_game->object_server_.Add(enemy);
	}
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
}