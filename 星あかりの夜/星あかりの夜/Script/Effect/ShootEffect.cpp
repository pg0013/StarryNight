/**
 * @file    ShootChargeEffect.h
 * @brief  射撃構え時のエフェクトクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/09
 */

#include "ShootEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::effect;

ShootEffect::ShootEffect()
{
	effect_resource_ = LoadEffekseerEffect("Resource/effect/shoot.efk", 10.0f);
	effect_frame_ = 120;
}

ShootEffect::~ShootEffect()
{
}

void ShootEffect::Initialize()
{
	EffectBase::Initialize();

	//プレイヤーの座標と回転ベクトルを取得
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));

	//プレイヤーの向きベクトル
	VECTOR right = VNorm(utility::GetRightVector(player_rotation.y));
	VECTOR up = VNorm(utility::GetUpVector(player_rotation.y));
	VECTOR back = VScale(VNorm(utility::GetForwardVector(player_rotation.y)), -1.0f);

	//プレイヤーのパチンコの位置にエフェクトを配置
	VECTOR effect_pos;
	effect_pos = VAdd(player_position, VScale(right, 20.0f));
	effect_pos = VAdd(effect_pos, VScale(up, 110.0f));
	effect_pos = VAdd(effect_pos, VScale(back, 10.0f));

	SetPosition(effect_pos);

	//カメラターゲットとプレイヤーの角度を算出
	VECTOR target = camera::Camera::GetInstance()->GetTarget();
	float diff_x = abs(camera::Camera::GetInstance()->GetClip().far_) - abs(player_position.x);
	float diff_y = target.y - player_position.y;

	float tar_rad_xy = atan2(diff_y, diff_x);
	float tar_rad_xz = player_rotation.y + DEG2RAD(90.0f);

	//EffekseerとDXライブラリで回転する軸の順番が違う
	//回転の方向は時計回りである。
	//回転の順番は Z軸回転 → X軸回転 → Y軸回転
	rotation_.x = 0.0f;
	rotation_.y = tar_rad_xz;
	rotation_.z = tar_rad_xy;

	SetPlayingEffectPosition();
	SetPlayingEffectRotation();
}

void ShootEffect::Process()
{
	EffectBase::Process();

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	SetPlayingEffectPosition();
	SetPlayingEffectRotation();

	//カメラが射撃モードでなくなったら、エフェクトを削除
	if (camera::Camera::GetInstance()->GetStatus() != camera::Camera::STATUS::SHOOT)
	{
		mode::ModeGame* mode_game =
			static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
		mode_game->effect_server_.Delete(this);
	}
}

void ShootEffect::Render()
{
	EffectBase::Render();
}