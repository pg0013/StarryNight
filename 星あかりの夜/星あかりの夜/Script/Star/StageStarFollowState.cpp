/**
 * @file    StageStarFollowState.cpp
 * @brief  フィールド内スターの追跡状態クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include "StageStarFollowState.h"
#include"StageStar.h"
#include"../Player/Player.h"
#include"../Mode/ModeGame.h"
#include"../Effect/GetStarEffect.h"

using namespace starrynight::star;

StageStarFollowState::StageStarFollowState()
{
	follow_interval_ = 100.0f;
	old_player_position_ = { 0,0,0 };
}

StageStarFollowState::~StageStarFollowState()
{
}

void StageStarFollowState::Enter(Star& _star)
{
	//取得したスターの数を増やす
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();
	mode_game->AddPlayerStarNum();
	_star.SetPlayersStarNum(mode_game->GetPlayerStarNum());

	VECTOR position = _star.GetPosition();
	VECTOR rotation = _star.GetRotation();

	VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));

	player_position = VAdd(player_position, VGet(0.0f, 50.0f, 0.0f));
	//プレイヤーの後ろに星を移動
	position = VAdd(player_position, VScale(VNorm(utility::GetForwardVector(player_rotation.y)), -0.5f * follow_interval_));

	//星獲得エフェクトを生成
	effect::GetStarEffect* getstar_effect = NEW effect::GetStarEffect();
	getstar_effect->SetPosition(player_position);
	getstar_effect->Initialize();
	getstar_effect->PlayEffect();
	mode_game->effect_server_.Add(getstar_effect);

	//連れている星を見ると回転が少しずれているように、回転値を設定する
	rotation.y = DEG2RAD(45.0f * _star.GetPlayersStarNum());

	_star.se_.Load("Resource/sound/getstar.wav");
	_star.se_.Play();

	_star.SetPosition(position);
	_star.SetRotation(rotation);
}

void StageStarFollowState::Exit(Star& _star)
{
	_star.SetPlayersStarNum(0);
	old_player_position_ = { 0,0,0 };

	//queueを初期化
	std::queue<VECTOR> empty;
	std::swap(player_pos_history_, empty);
}

void StageStarFollowState::Input(Star& _star)
{
	//プレイヤーがダメージを受けたかどうかを取得
	bool player_damaged = false;
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();

	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(iter);
			player_damaged = player->GetDamageFlag();
			break;
		}
	}

	if (player_damaged == true)
	{
		_star.ChangeStageStarState("Diffusion");
	}
}

void StageStarFollowState::Update(Star& _star)
{
	Follow(_star);
}