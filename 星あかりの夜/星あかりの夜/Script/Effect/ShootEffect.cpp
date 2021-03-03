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

ShootEffect::ShootEffect(VECTOR _target)
{
	effect_resource_ = LoadEffekseerEffect("Resource/effect/shoot.efk", 10.0f);
	effect_frame_ = 120;
	shoot_target_ = _target;
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

	//エフェクトがx軸正方向に向いているので、ターゲットとx軸の
	//xz平面の角度を求める
	VECTOR axis_x = VGet(1, 0, 0);
	VECTOR totarget = VNorm(VGet(shoot_target_.x - effect_pos.x, 0, shoot_target_.z - effect_pos.z));
	float theta = VDot(axis_x, totarget);
	theta = acosf(theta);

	//フォワードベクトルとエフェクトからターゲットへ向くベクトルの角度を求める
	VECTOR forward = VSub(VAdd(utility::GetForwardVector(player_rotation.y), effect_pos), effect_pos);
	totarget = VNorm(VSub(shoot_target_, effect_pos));
	float phi = VDot(forward, totarget);
	phi = acosf(phi);

	rotation_.x = 0.0f;
	rotation_.y = -theta;//反時計回りの回転をする行列なのでマイナス倍する
	rotation_.z = phi;

	Effekseer_Sync3DSetting();

	//effekseerをもちいて回転行列を生成
	Effekseer::Manager* manager = GetEffekseer3DManager();
	Effekseer::Matrix43 matrix, rot_matrix;
	rot_matrix.Indentity();
	rot_matrix.RotationZXY(rotation_.z, rotation_.x, rotation_.y);

	//エフェクトの行列を取得し、回転行列をセットする
	matrix = manager->GetBaseMatrix(playing_effect_);
	matrix.SetSRT(DXLibtoEffekseer(VGet(10.0f, 10.0f, 10.0f)),
		rot_matrix,
		DXLibtoEffekseer(effect_pos));
	manager->SetBaseMatrix(playing_effect_, matrix);
}

void ShootEffect::Process()
{
	EffectBase::Process();

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

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