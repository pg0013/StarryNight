/**
 * @file    StarAction.cpp
 * @brief  スターの拡散処理
 *
 * @author Takuya Fujisawa
 * @date   2021/03/13
 */

#include "Star.h"
#include"../Stage/Stage.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::star;

void Star::Diffusion()
{
	VECTOR move = VNorm(utility::GetForwardVector(rotation_.y));
	move = VScale(move, 10.0f);

	//ステージ範囲外まで到達したら、移動しない
	float out_of_stage = 2300.0f;
	if (VSize(VAdd(position_, move)) > out_of_stage)
		move = VAdd(move, VScale(VNorm(VSub(VGet(0, 0, 0), position_)), 50.0f));

	//プレイヤーのカプセル情報
	VECTOR sphere_positon = VAdd(position_, VGet(0, 50, 0));
	float radius = 50.0f;

	//乗れるオブジェクトとの当たり判定
	MV1_COLL_RESULT_POLY_DIM hit_sphere_wall;
	hit_sphere_wall = stage::Stage::GetInstance()->GetHitSphereToWall(sphere_positon, radius);

	//ジャンプ中の壁刷り処理
	if (hit_sphere_wall.HitNum > 0)
	{
		VECTOR normal = { 0,0,0 };
		for (int i = 0; i < hit_sphere_wall.HitNum; i++)
		{
			normal = VAdd(normal, hit_sphere_wall.Dim[i].Normal);
		}
		normal = VNorm(normal);
		VECTOR fall = { 0 , jump_speed_ , 0 };

		move = VAdd(move, fall);

		VECTOR escape = VCross(move, normal);
		escape = VCross(normal, escape);

		position_ = VAdd(position_, escape);
	}
	else
	{
		position_ = VAdd(position_, move);
		position_.y += jump_speed_;
	}
	MV1CollResultPolyDimTerminate(hit_sphere_wall);

	jump_speed_ -= gravity_ * 3.0f;

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;

	//地面までの線分ベクトル
	VECTOR start_line = position_;
	VECTOR end_line = VAdd(position_, VGet(0, -50.0f, 0));

	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

	if (hit_poly_floor.HitFlag)
	{
		ground_position_y_ = position_.y;
		status_ = STATUS::WAIT;
	}
}