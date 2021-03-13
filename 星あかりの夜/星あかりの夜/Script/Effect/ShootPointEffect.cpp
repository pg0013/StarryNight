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
	effect_resource_ = LoadEffekseerEffect("Resource/Effect/shootpoint.efk", 1.0f);
	effect_frame_ = 600;

	scale_y_ = 30.0f;
	min_scale_y_ = 25.0f;
	max_scale_y_ = 80.0f;

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

	Effekseer::Manager* manager = GetEffekseer3DManager();
	Effekseer::Matrix43 matrix, rot_matrix;
	rot_matrix.Indentity();
	matrix = manager->GetBaseMatrix(playing_effect_);

	//プレイヤーが星発射可能状態の時に発射ポイントに近づいたら、エフェクトを縦につぶす
	if (VSize(distance) < turnoff_length &&
		mode_game->GetPlayerStarNum() > 0)
	{
		scale_y_--;
		if (scale_y_ < min_scale_y_)
			scale_y_ = min_scale_y_;

		matrix.SetSRT(DXLibtoEffekseer(VGet(30.0f, scale_y_, 30.0f)),
			rot_matrix,
			DXLibtoEffekseer(position_));
		manager->SetBaseMatrix(playing_effect_, matrix);
	}
	else
	{
		//プレイヤーが遠くにいるときには、発射ポイントが分かるように
		//エフェクトを縦に伸ばす
		scale_y_++;
		if (scale_y_ > max_scale_y_)
			scale_y_ = max_scale_y_;

		matrix.SetSRT(DXLibtoEffekseer(VGet(30.0f, scale_y_, 30.0f)),
			rot_matrix,
			DXLibtoEffekseer(position_));
		manager->SetBaseMatrix(playing_effect_, matrix);
	}
}

void ShootPointEffect::Render()
{
	EffectBase::Render();
}