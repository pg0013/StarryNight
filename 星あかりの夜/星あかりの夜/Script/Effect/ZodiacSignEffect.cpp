/**
 * @file    ZodiacSignEffect.cpp
 * @brief  星座エフェクトクラス
 *
 * @author Takuya Fujisawa
 * @date    202103/02
 */

#include "ZodiacSignEffect.h"
#include"TimingRankEffect.h"
#include"../Camera/Camera.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::effect;

ZodiacSignEffect::ZodiacSignEffect(std::string _sign_name)
{
	skystar_name_ = _sign_name;
	std::string filename = "Resource/effect/" + _sign_name + "_before.efk";
	effect_resource_ = LoadEffekseerEffect(filename.c_str(), 1.0f);
	filename = "Resource/effect/" + _sign_name + "_after01.efk";
	after_effect_resource_[0] = LoadEffekseerEffect(filename.c_str(), 1.0f);
	filename = "Resource/effect/" + _sign_name + "_after02.efk";
	after_effect_resource_[1] = LoadEffekseerEffect(filename.c_str(), 1.0f);
	filename = "Resource/effect/" + _sign_name + "_after03.efk";
	after_effect_resource_[2] = LoadEffekseerEffect(filename.c_str(), 1.0f);

	resource::ResourceServer::LoadSound("Resource/sound/skystar_complete.wav");

	effect_frame_ = 120;
	once_flag_ = false;
	start_frame_ = 0;
	select_star_num_ = -1;

	switch_effect_frame_ = 120;
	if (_sign_name == "ohitsuji")
	{
		complete_effect_startframe_ = 50;
	}
	else if (_sign_name == "oushi")
	{
		complete_effect_startframe_ = 110;
	}
	else if (_sign_name == "futago")
	{
		complete_effect_startframe_ = 80;
	}
}

ZodiacSignEffect::~ZodiacSignEffect()
{
	for (int i = 0; i < 3; i++)
	{
		//使用したエフェクトは基底クラスで消す
		if (i == select_star_num_) { continue; }
		DeleteEffekseerEffect(after_effect_resource_[i]);
	}

	if (se1_.CheckIsRunning())
	{
		se1_.Pause();
		se1_.Destroy();
	}
	if (se2_.CheckIsRunning())
	{
		se2_.Pause();
		se2_.Destroy();
	}
}

void ZodiacSignEffect::Initialize()
{
	EffectBase::Initialize();
}

void ZodiacSignEffect::Process()
{
	EffectBase::Process();

	DrawCompleteEffect();
	PlayEffectSound();

	VECTOR skystar_position = MV1GetPosition(resource::ResourceServer::GetModelHandle(skystar_name_));
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
	{
		start_frame_ = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount();
	}

	int elapsed_frame = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - start_frame_;

	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	if (elapsed_frame == switch_effect_frame_)
	{
		StopEffekseer3DEffect(playing_effect_);
		DeleteEffekseerEffect(effect_resource_);

		//何番星が選択されたかでエフェクトを変える
		select_star_num_ = mode_game->ui_.timing_ui_.GetSelectedStarNum() - 1;
		effect_resource_ = after_effect_resource_[select_star_num_];

		PlayEffect();
		SetPosition(VGet(0, 0, 0));
		SetPlayingEffectPosition();

		SetSpeedPlayingEffekseer3DEffect(playing_effect_, 1.0f);
	}

	if (elapsed_frame == switch_effect_frame_ + complete_effect_startframe_)
	{
		//星獲得エフェクトを生成
		effect::TimingRankEffect* timing_effect = NEW effect::TimingRankEffect(mode_game->GetScoreRank());
		timing_effect->SetPosition(position_);
		timing_effect->Initialize();
		timing_effect->PlayEffect();
		mode_game->effect_server_.Add(timing_effect);
		once_flag_ = true;
	}
}

void ZodiacSignEffect::PlayEffectSound()
{
	if (camera::Camera::GetInstance()->GetStatus() != camera::Camera::STATUS::SKYSTAR)
		return;

	int elapsed_frame = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - start_frame_;

	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	if (elapsed_frame == switch_effect_frame_ + complete_effect_startframe_)
	{
		se1_.Load("Resource/sound/skystar_complete.wav");
		se1_.Play();
		se1_.Fade(0.0f, 2.0f);
		appframe::ApplicationBase::GetInstance()->bgm_.Pause();
	}

	if (skystar_name_ == "ohitsuji")
	{
		if (elapsed_frame == switch_effect_frame_ + 0)
		{
			se2_.Load("Resource/sound/skystar_make.wav");
			se2_.Play();
		}
		if (elapsed_frame == switch_effect_frame_ + 10)
		{
			se2_.Load("Resource/sound/skystar_make.wav");
			se2_.Play();
		}
		if (elapsed_frame == switch_effect_frame_ + 30)
		{
			se2_.Load("Resource/sound/skystar_make.wav");
			se2_.Play();
		}
		if (elapsed_frame == switch_effect_frame_ + complete_effect_startframe_ + 30)
		{
			se2_.Load("Resource/sound/skystar_particle.wav");
			se2_.Play();
			se1_.Fade(1.0f, 1.0f);
		}
	}
	else if (skystar_name_ == "oushi")
	{
		if (elapsed_frame == switch_effect_frame_ + 0)
		{
			se2_.Load("Resource/sound/skystar_make.wav");
			se2_.Play();
		}
		if (elapsed_frame == switch_effect_frame_ + 40)
		{
			se2_.Load("Resource/sound/skystar_make.wav");
			se2_.Play();
		}
		if (elapsed_frame == switch_effect_frame_ + 50)
		{
			se2_.Load("Resource/sound/skystar_make.wav");
			se2_.Play();
		}
		if (elapsed_frame == switch_effect_frame_ + complete_effect_startframe_ + 30)
		{
			se2_.Load("Resource/sound/skystar_particle.wav");
			se2_.Play();
			se1_.Fade(1.0f, 1.0f);
		}
	}
	else if (skystar_name_ == "futago")
	{
		if (elapsed_frame == switch_effect_frame_ + 0)
		{
			se2_.Load("Resource/sound/skystar_make.wav");
			se2_.Play();
		}
		if (elapsed_frame == switch_effect_frame_ + 10)
		{
			se2_.Load("Resource/sound/skystar_make.wav");
			se2_.Play();
		}
		if (elapsed_frame == switch_effect_frame_ + 40)
		{
			se2_.Load("Resource/sound/skystar_make.wav");
			se2_.Play();
		}
		if (elapsed_frame == switch_effect_frame_ + complete_effect_startframe_ + 30)
		{
			se2_.Load("Resource/sound/skystar_particle.wav");
			se2_.Play();
			se1_.Fade(1.0f, 1.0f);
		}
	}
}