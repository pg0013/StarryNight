/**
 * @file    CollisionToStage.cpp
 * @brief  ステージとのコリジョン判定処理
 *
 * @author Takuya Fujisawa
 * @date    202101/18
 */

#include"Stage.h"
using namespace starrynight::stage;

MV1_COLL_RESULT_POLY Stage::GetHitLineToFloor(VECTOR& _startline, VECTOR& _endline)
{
	MV1_COLL_RESULT_POLY hit_poly;
	hit_poly.HitFlag = 0;
	hit_poly.HitPosition = { 0,-1,0 };

	MV1_COLL_RESULT_POLY hit_poly_check;
	hit_poly_check.HitFlag = 0;

	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		//床ポリゴンがあるオブジェクトと当たり判定をとる
		if (MV1SearchFrame((*iter), "floor_NavMesh") > 0)
		{
			hit_poly_check = MV1CollCheck_Line(
				(*iter),
				MV1SearchFrame((*iter), "floor_NavMesh"),
				_startline, _endline);
		}

		if (!hit_poly_check.HitFlag)
			continue;

		//すでに当たり判定があるものより高さが高ければ、入れ替える
		if (hit_poly_check.HitPosition.y >= hit_poly.HitPosition.y)
		{
			std::swap(hit_poly, hit_poly_check);
		}
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY Stage::GetHitLineToWall(VECTOR& _startline, VECTOR& _endline)
{
	MV1_COLL_RESULT_POLY hit_poly;
	hit_poly.HitFlag = 0;

	//壁ポリゴンがあるオブジェクトと当たり判定をとる
	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		if (MV1SearchFrame((*iter), "wall_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Line(
				(*iter),
				MV1SearchFrame((*iter), "wall_NavMesh"),
				_startline, _endline);
		}

		//当たり判定があれば抜ける
		if (hit_poly.HitFlag)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitSphereToFloor(VECTOR& _position, float _radius)
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		//床ポリゴンがあるオブジェクトと当たり判定をとる
		if (MV1SearchFrame((*iter), "floor_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Sphere(
				(*iter),
				MV1SearchFrame((*iter), "floor_NavMesh"),
				_position,
				_radius);
		}

		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitSphereToWall(VECTOR& _position, float _radius)
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	//壁ポリゴンがあるオブジェクトと当たり判定をとる
	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		if (MV1SearchFrame((*iter), "wall_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Sphere(
				(*iter),
				MV1SearchFrame((*iter), "wall_NavMesh"),
				_position,
				_radius);
		}

		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitCapsuleToFloor(VECTOR& _position1, VECTOR& _position2, float _radius)
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	//床ポリゴンがあるオブジェクトと当たり判定をとる
	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		if (MV1SearchFrame((*iter), "floor_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Capsule(
				(*iter),
				MV1SearchFrame((*iter), "floor_NavMesh"),
				_position1, _position2, _radius);
		}

		//当たり判定があれば抜ける
		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitCapsuleToWall(VECTOR& _position1, VECTOR& _position2, float _radius)
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	//壁ポリゴンがあるオブジェクトと当たり判定をとる
	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		if (MV1SearchFrame((*iter), "wall_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Capsule(
				(*iter),
				MV1SearchFrame((*iter), "wall_NavMesh"),
				_position1, _position2, _radius);
		}

		//当たり判定があれば抜ける
		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY Stage::GetHitLineToShootPoint(VECTOR& _startline, VECTOR& _endline)
{
	MV1_COLL_RESULT_POLY hit_poly;

	hit_poly = MV1CollCheck_Line(
		shootpoint_,
		MV1SearchFrame(shootpoint_, "floor_NavMesh"),
		_startline, _endline);

	return hit_poly;
}