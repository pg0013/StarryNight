/**
 * @file    Star.cpp
 * @brief  ステージ内のスター挙動実装クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/01/23
 */

#include "Star.h"
#include"../Stage/Stage.h"

using namespace starrynight::star;

Star::Star()
{
	status_ = STATUS::WAIT;
	rot_speed_ = 5.0f;
	jump_height_ = 5.0f;
	jump_speed_ = jump_height_;
	gravity_ = 0.1f;
}

Star::~Star()
{
}

void Star::Initialize()
{
	ground_position_y_ = position_.y;
}

void Star::Process()
{
	switch (status_)
	{
	case starrynight::star::Star::STATUS::NONE:
		break;
	case starrynight::star::Star::STATUS::WAIT:
		Wait();
		break;
	case starrynight::star::Star::STATUS::FOLLOW:
		Follow();
		break;
	case starrynight::star::Star::STATUS::DIFFUSION:
		Diffusion();
		break;
	case starrynight::star::Star::STATUS::_EOT_:
		break;
	default:
		break;
	}
}

void Star::Wait()
{
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));

	VECTOR distance = VSub(player_position, position_);
	float length = VSize(distance);
	float detect_length = 500.0f;

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;

	//腰から地面までの線分ベクトル
	VECTOR start_line = position_;
	VECTOR end_line = VAdd(position_, VGet(0, -10.0f, 0));

	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

	if (length < detect_length)
	{
		if (position_.y < ground_position_y_)
		{
			position_.y = ground_position_y_;
			jump_speed_ = jump_height_;
		}

		position_.y += jump_speed_;
		jump_speed_ -= gravity_;

		rotation_.y += DEG2RAD(rot_speed_);
	}
	else
	{
		if (jump_speed_ >= -jump_height_)
		{
			position_.y += jump_speed_;
			jump_speed_ -= gravity_;
		}
		else
		{
			position_.y = ground_position_y_;
		}
	}
}

void Star::Follow()
{
}

void Star::Diffusion()
{
}

void Star::Render()
{
	MV1SetPosition(handle_, position_);
	MV1SetRotationXYZ(handle_, rotation_);
	MV1DrawModel(handle_);
}