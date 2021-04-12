/**
 * @file    Move.cpp
 * @brief  エネミーの移動処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include "Enemy.h"
#include"../Mode/ModeGame.h"
#include"../Stage/Stage.h"

using namespace starrynight::enemy;

void Enemy::Move(const VECTOR& _move)
{
	VECTOR old_position = position_;
	position_ = VAdd(position_, _move);

	//当たり判定用の球を設定
	VECTOR sphere_positon = VAdd(position_, VGet(0, 45, 0));
	float radius = 35.0f;

	//壁との当たり判定を取得
	MV1_COLL_RESULT_POLY_DIM hit_poly_wall;
	hit_poly_wall = stage::Stage::GetInstance()->GetHitSphereToWall(sphere_positon, radius);

	//壁ずり処理
	if (hit_poly_wall.HitNum > 0)
	{
		VECTOR normal = { 0,0,0 };
		for (int i = 0; i < hit_poly_wall.HitNum; i++)
		{
			normal = VAdd(normal, hit_poly_wall.Dim[i].Normal);
		}
		normal = VNorm(normal);

		//かべずりベクトルを計算
		VECTOR escape = VCross(_move, normal);
		escape = VCross(normal, escape);
		escape.y = 0.0f;

		position_ = VAdd(position_, escape);

		MV1CollResultPolyDimTerminate(hit_poly_wall);

		//押し出し処理
		while (1)
		{
			sphere_positon = VAdd(position_, VGet(0, 45, 0));

			//壁ずりをした位置と当たり判定をとり、のめりこみを判定
			MV1_COLL_RESULT_POLY_DIM hit_poly_wallpush;
			hit_poly_wallpush = stage::Stage::GetInstance()->GetHitSphereToWall(sphere_positon, radius);

			//のめりこんでいないので終了
			if (hit_poly_wallpush.HitNum == 0)
				break;

			normal = { 0,0,0 };
			for (int i = 0; i < hit_poly_wall.HitNum; i++)
			{
				normal = VAdd(normal, VNorm(hit_poly_wall.Dim[i].Normal));
			}
			normal = VNorm(normal);

			//壁の法線ベクトルを足していき、壁の外へ押し出す
			position_ = VAdd(position_, VScale(normal, 0.5f));

			MV1CollResultPolyDimTerminate(hit_poly_wallpush);
		}
	}

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;

	//腰から地面までの線分ベクトル
	VECTOR start_line = VAdd(position_, VGet(0, 10.0f, 0));
	VECTOR end_line = VAdd(position_, VGet(0, -10.0f, 0));

	//床との当たり判定を取得
	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

	//床との判定があれば接地し、無ければ移動せず待機
	if (hit_poly_floor.HitFlag)
		position_.y = hit_poly_floor.HitPosition.y;
	else
	{
		position_ = old_position;
		anim_status_ = ANIM_STATUS::WAIT;
	}
}