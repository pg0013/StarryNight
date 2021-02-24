/**
 * @file    ShootPointEffect.h
 * @brief  射撃ポイントのエフェクトクラス
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

	//プレイヤーとの距離を算出
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR distance = VSub(player_position, position_);

	//プレイヤーとの距離が近くなったらエフェクトを停止
	float turnoff_length = 200.0f;

	if (VSize(distance) < turnoff_length)
	{
		//プレイヤーが星を持っていれば、エフェクトを停止
		if (mode_game->GetPlayerStarNum() > 0)
		{
			StopEffekseer3DEffect(playing_effect_);
			once_flag_ = true;
		}
	}
	else
	{
		//1フレームだけ処理が行われるようにフラグで管理
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