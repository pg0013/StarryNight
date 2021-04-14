/**
 * @file    StarAction.cpp
 * @brief  スターの拡散処理
 *
 * @author Takuya Fujisawa
 * @date   2021/03/13
 */

#include "StageStarDiffusionState.h"
#include"../Stage/Stage.h"
#include"StageStar.h"

using namespace starrynight::star;

void StageStarDiffusionState::Diffusion(Star& _star)
{
	VECTOR position = _star.GetPosition();
	VECTOR rotation = _star.GetRotation();

	VECTOR move = VNorm(utility::GetForwardVector(rotation.y));
	move = VScale(move, 10.0f);

	//ステージ範囲外まで到達したら、移動しない
	float out_of_stage = 2300.0f;
	if (VSize(VAdd(position, move)) > out_of_stage)
		move = VAdd(move, VScale(VNorm(VSub(VGet(0, 0, 0), position)), 50.0f));

	//プレイヤーのカプセル情報
	VECTOR sphere_positon = VAdd(position, VGet(0, 50, 0));
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
		VECTOR fall = { 0 , _star.GetJumpSpeed() , 0 };

		move = VAdd(move, fall);

		VECTOR escape = VCross(move, normal);
		escape = VCross(normal, escape);

		position = VAdd(position, escape);
	}
	else
	{
		position = VAdd(position, move);
		position.y += _star.GetJumpSpeed();
	}
	MV1CollResultPolyDimTerminate(hit_sphere_wall);

	//重力を強くし、着地を早める
	_star.SetJumpSpeed(_star.GetJumpSpeed() - (_star.GetGravity() * 3.0f));

	//座標と回転角度を更新
	_star.SetPosition(position);
	_star.SetRotation(rotation);
}