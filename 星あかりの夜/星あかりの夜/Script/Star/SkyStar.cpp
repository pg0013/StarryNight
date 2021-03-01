/**
 * @file    SkyStar.cpp
 * @brief  射撃モードで射る星のクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/27
 */

#include "SkyStar.h"

using namespace starrynight::star;

SkyStar::SkyStar(const std::string _name)
{
	SetModelHandle(resource::ResourceServer::GetModelHandle(_name));
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
	float height = 8000.0f;
	float degree = 45.0f;

	position_.x = radius * cosf(DEG2RAD(degree));
	position_.z = radius * sinf(DEG2RAD(degree));
	position_.y = height;

	MV1SetPosition(handle_, position_);

	//1番星～3番星まで当たり判定をとる
	for (int i = 1; i <= 3; i++)
	{
		MV1SetupCollInfo(handle_, i, 16, 16, 16);
		MV1SetFrameVisible(handle_, i, TRUE);
	}
}

void SkyStar::Process()
{
	for (int i = 1; i <= 3; i++)
	{
		MV1RefreshCollInfo(handle_, i);
	}
}

void SkyStar::SetRotationToPlayer()
{
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));

	//モデルが向くz軸とy軸を求める
	VECTOR forward = VNorm(VNorm(VSub(player_position, position_)));
	forward = VScale(forward, -1.0f);
	VECTOR up = VGet(0, 1, 0);
	VECTOR right = VCross(forward, up);
	up = VCross(right, forward);

	//モデルをプレイヤー方向に向ける
	MV1SetRotationZYAxis(handle_, forward, up, 0.0f);
	rotation_ = MV1GetRotationXYZ(handle_);
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
	MV1SetPosition(handle_, position_);
	SetRotationToPlayer();
	MV1RefreshCollInfo(handle_, MV1SearchFrame(handle_, "wall_NavMesh"));

	ObjectBase::Render();
}