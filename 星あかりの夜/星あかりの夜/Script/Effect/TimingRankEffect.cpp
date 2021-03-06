/**
 * @file    TimingRankEffect.h
 * @brief  タイミング判定によって変わる射撃時のエフェクト
 *
 * @author Takuya Fujisawa
 * @date   2021/03/05
 */

#include "TimingRankEffect.h"

using namespace starrynight::effect;

TimingRankEffect::TimingRankEffect(mode::ModeGame::SCORE_RANK _rank)
{
	switch (_rank)
	{
	case mode::ModeGame::SCORE_RANK::HIGH:
		effect_resource_ = LoadEffekseerEffect("Resource/Effect/timing_excellent.efk", 400.0f);
		break;
	case mode::ModeGame::SCORE_RANK::MIDDLE:
		effect_resource_ = LoadEffekseerEffect("Resource/Effect/timing_good.efk", 400.0f);
		break;
	case mode::ModeGame::SCORE_RANK::LOW:
		effect_resource_ = LoadEffekseerEffect("Resource/Effect/timing_bad.efk", 400.0f);
		break;
	}
	effect_frame_ = 240;
	start_frame_ = 0;
}

TimingRankEffect::~TimingRankEffect()
{
}

void TimingRankEffect::Initialize()
{
	start_frame_ = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount();
	SetSpeedPlayingEffekseer3DEffect(playing_effect_, 2.0f);
}

void TimingRankEffect::Process()
{
	EffectBase::Process();

	//position_ = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	//position_ = VAdd(position_, VGet(0, 60, 0));

	int elapsed_frame = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - start_frame_;

	//再生完了したらエフェクトを削除
	if (elapsed_frame > effect_frame_)
	{
		mode::ModeGame* mode_game =
			static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
		mode_game->effect_server_.Delete(this);
	}

	SetPlayingEffectPosition();
}

void TimingRankEffect::Render()
{
	EffectBase::Render();
}