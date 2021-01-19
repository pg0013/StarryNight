/**
 * @file    CollisionToStage.cpp
 * @brief  ステージとのコリジョン判定処理
 *
 * @author Takuya Fujisawa
 * @date    202101/18
 */

#include"Stage.h"
using namespace starrynight::stage;

MV1_COLL_RESULT_POLY Stage::GetHitLineToNaviMesh(VECTOR& _startline, VECTOR& _endline)
{
	MV1_COLL_RESULT_POLY hit_poly;
	hit_poly.HitFlag = 0;

	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		if (MV1SearchFrame((*iter), "field_B_NavMesh_GEO") > 0)
		{
			hit_poly = MV1CollCheck_Line(
				(*iter),
				MV1SearchFrame((*iter), "field_B_NavMesh_GEO"),
				_startline, _endline);
		}

		//当たり判定があれば抜ける
		if (hit_poly.HitFlag)
			break;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY Stage::GetHitLineToColObject(VECTOR& _startline, VECTOR& _endline)
{
	MV1_COLL_RESULT_POLY hit_poly;
	hit_poly.HitFlag = 0;

	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		if (MV1SearchFrame((*iter), "foothold_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Line(
				(*iter),
				MV1SearchFrame((*iter), "foothold_NavMesh"),
				_startline, _endline);
		}
		if (MV1SearchFrame((*iter), "step_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Line(
				(*iter),
				MV1SearchFrame((*iter), "step_NavMesh"),
				_startline, _endline);
		}

		//当たり判定があれば抜ける
		if (hit_poly.HitFlag)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitSphereToStage(VECTOR& _position, float _radius)
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		if (MV1SearchFrame((*iter), "field_B_NavMesh_GEO") > 0)
		{
			hit_poly = MV1CollCheck_Sphere(
				(*iter),
				MV1SearchFrame((*iter), "field_B_NavMesh_GEO"),
				_position,
				_radius);
		}

		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitSphereToColObject(VECTOR& _position, float _radius)
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		if (MV1SearchFrame((*iter), "foothold_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Sphere(
				(*iter),
				MV1SearchFrame((*iter), "foothold_NavMesh"),
				_position,
				_radius);
		}
		if (MV1SearchFrame((*iter), "step_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Sphere(
				(*iter),
				MV1SearchFrame((*iter), "step_NavMesh"),
				_position,
				_radius);
		}

		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitCapsuleToStage(VECTOR& _position1, VECTOR& _position2, float _radius)
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		if (MV1SearchFrame((*iter), "field_B_NavMesh_GEO") > 0)
		{
			hit_poly = MV1CollCheck_Capsule(
				(*iter),
				MV1SearchFrame((*iter), "field_B_NavMesh_GEO"),
				_position1, _position2, _radius);
		}

		//当たり判定があれば抜ける
		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitCapsuleToColObject(VECTOR& _position1, VECTOR& _position2, float _radius)
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	for (auto iter = navimesh_handle_.begin(); iter != navimesh_handle_.end(); iter++)
	{
		if (MV1SearchFrame((*iter), "foothold_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Capsule(
				(*iter),
				MV1SearchFrame((*iter), "foothold_NavMesh"),
				_position1, _position2, _radius);
		}
		if (MV1SearchFrame((*iter), "step_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Capsule(
				(*iter),
				MV1SearchFrame((*iter), "step_NavMesh"),
				_position1, _position2, _radius);
		}

		//当たり判定があれば抜ける
		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}