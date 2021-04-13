/**
 * @file    ShootChargeEffect.h
 * @brief  射撃構え時のエフェクトクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/09
 */

#include "ShootChargeEffect.h"
#include"../Mode/ModeGame.h"
#include"../Player/Player.h"

using namespace starrynight::effect;

ShootChargeEffect::ShootChargeEffect()
{
	effect_resource_ = LoadEffekseerEffect("Resource/effect/set.efk", 5.0f);
	effect_frame_ = 0;
}

ShootChargeEffect::~ShootChargeEffect()
{
}

void ShootChargeEffect::Initialize()
{
	EffectBase::Initialize();
}

void ShootChargeEffect::Process()
{
	EffectBase::Process();

	//一定間隔でSEを再生
	if (effect_frame_ % 30 == 0)
	{
		se_.Load("Resource/sound/star_charge.wav");
		se_.SetVolume_dB(-20.0f);
		se_.Play();
	}

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	//プレイヤーの座標と回転ベクトルを取得
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));

	//プレイヤーの向きベクトル
	VECTOR right = VNorm(utility::GetRightVector(player_rotation.y));
	VECTOR up = VNorm(utility::GetUpVector(player_rotation.y));
	VECTOR back = VScale(VNorm(utility::GetForwardVector(player_rotation.y)), -1.0f);

	//プレイヤーがパチンコを構えた手の場所にエフェクトを配置
	VECTOR effect_pos;
	effect_pos = VAdd(player_position, VScale(right, 20.0f));
	effect_pos = VAdd(effect_pos, VScale(up, 110.0f));
	effect_pos = VAdd(effect_pos, VScale(back, 10.0f));

	SetPosition(effect_pos);
	SetPlayingEffectPosition();

	//プレイヤーが射撃モードでないとき、または星が発射されたらエフェクトを消す
	mode::ModeGame* mode_game =
		mode::ModeGame::GetModeGame();

	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(iter);

			if (player->GetPlayerStatus() != player::Player::STATUS::SHOOT_START)
				mode_game->effect_server_.Delete(this);
		}
	}

	effect_frame_++;
}

void ShootChargeEffect::Render()
{
	EffectBase::Render();
}