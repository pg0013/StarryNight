/**
 * @file    SkyStar.cpp
 * @brief  射撃モードで射る星のクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/27
 */

#include "SkyStar.h"
#include"../Mode/ModeGame.h"
#include"../Effect/ZodiacSignEffect.h"

using namespace starrynight::star;

SkyStar::SkyStar(const std::string _name)
{
	zodiac_name_ = _name;
	SetModelHandle(resource::ResourceServer::GetModelHandle(zodiac_name_));
	position_ = MV1GetPosition(handle_);
	rotation_ = MV1GetRotationXYZ(handle_);

	//std::vector<std::string> param = { "walk_speed","run_speed","rot_speed" };
	//skystar_param_.LoadSkyStarParameter(_name);
}

SkyStar::~SkyStar()
{
}

void SkyStar::Initialize()
{
	float radius = 10000.0f;
	float height = 5000.0f;
	float degree = 180.0f;

	position_.x = radius * cosf(DEG2RAD(degree));
	position_.z = radius * sinf(DEG2RAD(degree));
	position_.y = height;

	MV1SetPosition(handle_, position_);

	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	effect::ZodiacSignEffect* zodiac_effect = NEW effect::ZodiacSignEffect(zodiac_name_);
	zodiac_effect->PlayEffect();
	zodiac_effect->SetPosition(VGet(0, 0, 0));
	zodiac_effect->SetPlayingEffectPosition();
	mode_game->effect_server_.Add(zodiac_effect);

	MV1SetFrameVisible(handle_, 0, FALSE);

	//1番星～3番星まで当たり判定をとる
	for (int i = 1; i <= 3; i++)
	{
		MV1SetupCollInfo(handle_, i, 16, 16, 16);
		MV1SetFrameVisible(handle_, i, FALSE);
	}
}

void SkyStar::Process()
{
	for (int i = 1; i <= 3; i++)
	{
		MV1RefreshCollInfo(handle_, i);
	}

	MV1SetPosition(handle_, position_);
	SetRotationToPlayer();
}

void SkyStar::SetRotationToPlayer()
{
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));

	//モデルが向くz軸とy軸を求める
	VECTOR forward = VNorm(VSub(player_position, position_));
	VECTOR up = VGet(0, 1, 0);
	VECTOR right = VCross(forward, up);
	up = VCross(right, forward);
	//モデルがz軸の負の方向を向いているため、
	std::swap(forward, right);

	MATRIX rot_matrix;
	rot_matrix = MGetIdent();
	rot_matrix.m[0][0] = forward.x;
	rot_matrix.m[0][1] = forward.y;
	rot_matrix.m[0][2] = forward.z;
	rot_matrix.m[1][0] = up.x;
	rot_matrix.m[1][1] = up.y;
	rot_matrix.m[1][2] = up.z;
	rot_matrix.m[2][0] = right.x;
	rot_matrix.m[2][1] = right.y;
	rot_matrix.m[2][2] = right.z;
	MV1SetRotationMatrix(handle_, rot_matrix);
}

MV1_COLL_RESULT_POLY SkyStar::GetHitToSkyStar(VECTOR _start, VECTOR _end)
{
	MV1_COLL_RESULT_POLY hit_poly;

	//1番星～3番星まで当たり判定をとる
	for (int i = 1; i <= 3; i++)
	{
		hit_poly = MV1CollCheck_Line(handle_, i,
			_start, _end);

		if (hit_poly.HitFlag)
			break;
	}

	return hit_poly;
}

void SkyStar::Render()
{
	ObjectBase::Render();
}