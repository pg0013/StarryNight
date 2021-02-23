/**
 * @file    EffectBase.cpp
 * @brief  エフェクトの基底クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/09
 */

#include "EffectBase.h"
using namespace starrynight::effect;

EffectBase::EffectBase()
{
}

EffectBase::~EffectBase()
{
	StopEffekseer3DEffect(playing_effect_);
	DeleteEffekseerEffect(effect_resource_);
}

void EffectBase::Initialize()
{
}

void EffectBase::Process()
{
}

void EffectBase::Render()
{
	Effekseer_Sync3DSetting();
	DrawEffekseer3D();
}

void EffectBase::PlayEffect()
{
	playing_effect_ = PlayEffekseer3DEffect(effect_resource_);
}

void EffectBase::SetPlayingEffectPosition()
{
	SetPosPlayingEffekseer3DEffect(playing_effect_, position_.x, position_.y, position_.z);
}

void EffectBase::SetPlayingEffectRotation()
{
	SetRotationPlayingEffekseer3DEffect(playing_effect_, rotation_.x, rotation_.y, rotation_.z);
}