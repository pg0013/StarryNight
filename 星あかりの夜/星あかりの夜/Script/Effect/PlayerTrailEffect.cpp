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
	effect_resource_ = LoadEffekseerEffect("Resource/Effect/player_trail.efk", 30.0f);
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

	//スティックの移動量と角度を計算
	float length = utility::GetLeftStickLength();

	//スティックの傾ける量で移動速度を変更
	if (length < ANALOG_MIN)
		length = 0.0f;

	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	//プレイヤーが星を持っていなければ、エフェクトを再生しない
	if (mode_game->GetPlayerStarNum() == 0 ||
		length == 0.0f)
	{
		//Effekseerの動的パラメータで、エフェクト生成時間を0に設定
		SetDynamicInput3DEffect(playing_effect_, 1, 0.0f);
		SetDynamicInput3DEffect(playing_effect_, 2, 0.0f);
		pause_flag_ = true;
	}
	else
	{
		if (pause_flag_)
		{
			//Effekseerの動的パラメータで、エフェクト生成時間を初期化
			SetDynamicInput3DEffect(playing_effect_, 1, 40.0f);
			SetDynamicInput3DEffect(playing_effect_, 2, 30.0f);
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