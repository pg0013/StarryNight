/**
 * @file    GetStarEffect.h
 * @brief  スターを獲得したときのエフェクトクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/03
 */

#include "GetStarEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::effect;

GetStarEffect::GetStarEffect()
{
	effect_resource_ = LoadEffekseerEffect("Resource/Effect/getstar.efk", 20.0f);
	effect_frame_ = 60;
	start_frame_ = 0;
}

GetStarEffect::~GetStarEffect()
{
}

void GetStarEffect::Initialize()
{
	//エフェクト生成時のフレームを取得
	start_frame_ = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount();
}

void GetStarEffect::Process()
{
	EffectBase::Process();

	position_ = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	position_ = VAdd(position_, VGet(0, 60, 0));

	int elapsed_frame = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - start_frame_;

	//再生完了したらエフェクトを削除
	if (elapsed_frame > effect_frame_)
	{
		std::shared_ptr<mode::ModeGame> mode_game =
			mode::ModeGame::GetModeGame();
		mode_game->effect_server_.Delete(shared_from_this());
	}

	SetPlayingEffectPosition();
}

void GetStarEffect::Render()
{
	EffectBase::Render();
}