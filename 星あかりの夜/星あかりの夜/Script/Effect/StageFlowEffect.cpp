/**
 * @file    StageFlowEffect.h
 * @brief  ステージ上に漂う花びらなどのエフェクト
 *
 * @author Takuya Fujisawa
 * @date   2021/03/03
 */

#include "StageFlowEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::effect;

StageFlowEffect::StageFlowEffect()
{
	effect_resource_ = LoadEffekseerEffect("Resource/effect/flower_effect.efk", 0.1f);
	effect_frame_ = 360;
}

StageFlowEffect::~StageFlowEffect()
{
	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	mode_game->effect_server_.Delete(this);
}

void StageFlowEffect::Initialize()
{
	EffectBase::Initialize();
}

void StageFlowEffect::Process()
{
	EffectBase::Process();

	SetPlayingEffectPosition();
}

void StageFlowEffect::Render()
{
	EffectBase::Render();
}