/**
 * @file    PlayerTrailEffect.h
 * @brief  プレイヤーの軌跡エフェクト
 *
 * @author Takuya Fujisawa
 * @date   2021/03/13
 */

#include "PlayerTrailEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::effect;

PlayerTrailEffect::PlayerTrailEffect()
{
	effect_resource_ = LoadEffekseerEffect("Resource/Effect/Trail_Test.efk", 20.0f);
	effect_frame_ = 60;
	pause_flag_ = false;
}

PlayerTrailEffect::~PlayerTrailEffect()
{
}

void PlayerTrailEffect::Initialize()
{
}

void PlayerTrailEffect::Process()
{
	EffectBase::Process();

	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	//プレイヤーが星を持っていなければ、エフェクトを再生しない
	if (mode_game->GetPlayerStarNum() == 0)
	{
		StopEffekseer3DEffect(playing_effect_);
		pause_flag_ = true;
	}
	else
	{
		if (pause_flag_)
		{
			PlayEffect();
			pause_flag_ = false;
		}
	}

	position_ = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	position_ = VAdd(position_, VGet(0, 60, 0));

	SetPlayingEffectPosition();
}

void PlayerTrailEffect::Render()
{
	EffectBase::Render();
}