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
	if (se_.CheckIsRunning())
	{
		se_.Pause();
		se_.Destroy();
	}
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
	SetRotationToTarget();

	se_.Load("Resource/sound/star_shoot.wav");
	se_.Play();
	se_.Fade(0.0f, 1.0f);
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

void ShootEffect::SetRotationToTarget()
{
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));

	VECTOR forward = VNorm(VSub(shoot_target_, position_));
	VECTOR up = VGet(0, 1, 0);
	VECTOR right = VCross(forward, up);
	up = VCross(right, forward);

	Effekseer_Sync3DSetting();

	//effekseerをもちいて回転行列を生成
	Effekseer::Manager* manager = GetEffekseer3DManager();
	Effekseer::Matrix43 matrix, rot_matrix;
	rot_matrix.Indentity();
	rot_matrix.Value[0][0] = forward.x;
	rot_matrix.Value[0][1] = forward.y;
	rot_matrix.Value[0][2] = forward.z;
	rot_matrix.Value[1][0] = up.x;
	rot_matrix.Value[1][1] = up.y;
	rot_matrix.Value[1][2] = up.z;
	rot_matrix.Value[2][0] = right.x;
	rot_matrix.Value[2][1] = right.y;
	rot_matrix.Value[2][2] = right.z;

	//エフェクトの行列を取得し、回転行列をセットする
	matrix = manager->GetBaseMatrix(playing_effect_);
	matrix.SetSRT(DXLibtoEffekseer(VGet(10.0f, 10.0f, 10.0f)),
		rot_matrix,
		DXLibtoEffekseer(position_));
	manager->SetBaseMatrix(playing_effect_, matrix);
}
