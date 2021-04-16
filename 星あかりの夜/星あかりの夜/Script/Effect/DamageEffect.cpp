/**
 * @file    DamageEffect.cpp
 * @brief  プレイヤーダメージエフェクト描画クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include "DamageEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::effect;

DamageEffect::DamageEffect()
{
	effect_resource_ = LoadEffekseerEffect("Resource/effect/damage.efk", 10.0f);
	effect_frame_ = 60;
}

DamageEffect::~DamageEffect()
{
}

void DamageEffect::Initialize()
{
	//エフェクト生成時のフレームを取得
	start_frame_ = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount();
}

void DamageEffect::Process()
{
	EffectBase::Process();

	SetPlayingEffectPosition();
	SetPlayingEffectRotation();

	int elapsed_frame = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - start_frame_;

	//再生が完了したらエフェクトを消去
	if (elapsed_frame > effect_frame_)
	{
		std::shared_ptr<mode::ModeGame> mode_game =
			mode::ModeGame::GetModeGame();
		mode_game->effect_server_.Delete(shared_from_this());
	}
}

void DamageEffect::Render()
{
	EffectBase::Render();
}