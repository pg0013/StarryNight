/**
 * @file    ShootPointEffect.h
 * @brief  射撃ポイントエフェクトクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/19
 */

#include "ShootPointEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::effect;

ShootPointEffect::ShootPointEffect()
{
	effect_resource_ = LoadEffekseerEffect("Resource/Effect/shootpoint.efk", 30.0f);
	effect_frame_ = 600;
	once_flag_ = false;
}

ShootPointEffect::~ShootPointEffect()
{
}

void ShootPointEffect::Initialize()
{
	EffectBase::Initialize();
}

void ShootPointEffect::Process()
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));


	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));

	VECTOR distance = VSub(player_position, position_);

	if (VSize(distance) < 200.0f)
	{
		if (mode_game->GetPlayerStarNum() > 0)
		{
			StopEffekseer3DEffect(playing_effect_);
			once_flag_ = true;
		}
	}
	else
	{
		if (once_flag_)
		{
			PlayEffect();
			once_flag_ = false;
		}
	}

	SetPlayingEffectPosition();
}

void ShootPointEffect::Render()
{
	EffectBase::Render();
}