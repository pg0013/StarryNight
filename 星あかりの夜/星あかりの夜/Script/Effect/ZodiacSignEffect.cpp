/**
 * @file    ZodiacSignEffect.cpp
 * @brief  星座エフェクトクラス
 *
 * @author Takuya Fujisawa
 * @date    202103/02
 */

#include "ZodiacSignEffect.h"
#include"../Camera/Camera.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::effect;

ZodiacSignEffect::ZodiacSignEffect(std::string _sign_name)
{
	skystar_name = _sign_name;
	std::string filename = "Resource/effect/" + _sign_name + "/" + _sign_name + "_before.efk";
	effect_resource_ = LoadEffekseerEffect(filename.c_str(), 1.0f);
	filename = "Resource/effect/" + _sign_name + "/" + _sign_name + "_after.efk";
	after_effect_resource_ = LoadEffekseerEffect(filename.c_str(), 1.0f);

	effect_frame_ = 120;
	once_flag_ = false;
	start_frame_ = 0;
}

ZodiacSignEffect::~ZodiacSignEffect()
{
}

void ZodiacSignEffect::Initialize()
{
	EffectBase::Initialize();
}

void ZodiacSignEffect::Process()
{
	EffectBase::Process();

	DrawCompleteEffect();

	VECTOR skystar_position = MV1GetPosition(resource::ResourceServer::GetModelHandle(skystar_name));
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));

	//星座判定とポリゴンが被らないように星座判定の手前に配置
	VECTOR distance = VSub(skystar_position, player_position);
	position_ = VAdd(player_position, VScale(distance, 1.0f));

	//プレイヤーを向く座標系を算出
	VECTOR forward = VNorm(VSub(player_position, position_));
	VECTOR up = VGet(0, 1, 0);
	VECTOR right = VCross(forward, up);
	up = VCross(right, forward);
	std::swap(forward, right);

	Effekseer_Sync3DSetting();

	//effekseerをもちいて回転行列を生成
	Effekseer::Manager* manager = GetEffekseer3DManager();
	Effekseer::Matrix43 matrix, rot_matrix;
	rot_matrix.Indentity();

	//プレイヤーの方向を向く回転行列を設定
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
	matrix.SetSRT(DXLibtoEffekseer(VGet(400.0f, 400.0f, 400.0f)),
		rot_matrix,
		DXLibtoEffekseer(position_));

	manager->SetBaseMatrix(playing_effect_, matrix);
}

void ZodiacSignEffect::Render()
{
	EffectBase::Render();
}

void ZodiacSignEffect::DrawCompleteEffect()
{
	if (once_flag_ == true)
		return;

	if (camera::Camera::GetInstance()->GetStatus() != camera::Camera::STATUS::SKYSTAR)
		return;

	if (start_frame_ == 0)
		start_frame_ = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount();

	int elapsed_frame = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - start_frame_;

	if (elapsed_frame > 120)
	{
		StopEffekseer3DEffect(playing_effect_);
		DeleteEffekseerEffect(effect_resource_);
		effect_resource_ = after_effect_resource_;

		PlayEffect();
		SetPosition(VGet(0, 0, 0));
		SetPlayingEffectPosition();

		SetSpeedPlayingEffekseer3DEffect(playing_effect_, 0.5f);

		once_flag_ = true;
	}
}