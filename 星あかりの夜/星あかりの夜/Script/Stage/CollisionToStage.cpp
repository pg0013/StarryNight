/**
 * @file    CollisionToStage.cpp
 * @brief  ステージとのコリジョン判定処理
 *
 * @author Takuya Fujisawa
 * @date    202101/18
 */

#include"Stage.h"
using namespace starrynight::stage;

MV1_COLL_RESULT_POLY Stage::GetHitLineToFloor(const VECTOR& _startline, const VECTOR& _endline) const
{
	MV1_COLL_RESULT_POLY hit_poly;
	hit_poly.HitFlag = 0;
	hit_poly.HitPosition = { 0,-1,0 };

	MV1_COLL_RESULT_POLY hit_poly_check;
	hit_poly_check.HitFlag = 0;

	for (auto&& iter : navimesh_handle_)
	{
		//床ポリゴンがあるオブジェクトと当たり判定をとる
		if (MV1SearchFrame(iter, "floor_NavMesh") > 0)
		{
			hit_poly_check = MV1CollCheck_Line(
				iter,
				MV1SearchFrame(iter, "floor_NavMesh"),
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

MV1_COLL_RESULT_POLY Stage::GetHitLineToWall(const VECTOR& _startline, const VECTOR& _endline) const
{
	MV1_COLL_RESULT_POLY hit_poly;
	hit_poly.HitFlag = 0;

	//壁ポリゴンがあるオブジェクトと当たり判定をとる
	for (auto&& iter : navimesh_handle_)
	{
		if (MV1SearchFrame(iter, "wall_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Line(
				iter,
				MV1SearchFrame(iter, "wall_NavMesh"),
				_startline, _endline);
		}

		//当たり判定があれば抜ける
		if (hit_poly.HitFlag)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitSphereToFloor(const VECTOR& _position, const float& _radius) const
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	for (auto&& iter : navimesh_handle_)
	{
		//床ポリゴンがあるオブジェクトと当たり判定をとる
		if (MV1SearchFrame((iter), "floor_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Sphere(
				(iter),
				MV1SearchFrame((iter), "floor_NavMesh"),
				_position,
				_radius);
		}

		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitSphereToWall(const VECTOR& _position, const float& _radius) const
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	//壁ポリゴンがあるオブジェクトと当たり判定をとる
	for (auto&& iter : navimesh_handle_)
	{
		if (MV1SearchFrame((iter), "wall_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Sphere(
				(iter),
				MV1SearchFrame((iter), "wall_NavMesh"),
				_position,
				_radius);
		}

		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitCapsuleToFloor(const VECTOR& _position1, const VECTOR& _position2, const float& _radius) const
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	//床ポリゴンがあるオブジェクトと当たり判定をとる
	for (auto&& iter : navimesh_handle_)
	{
		if (MV1SearchFrame((iter), "floor_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Capsule(
				(iter),
				MV1SearchFrame((iter), "floor_NavMesh"),
				_position1, _position2, _radius);
		}

		//当たり判定があれば抜ける
		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY_DIM Stage::GetHitCapsuleToWall(const VECTOR& _position1, const VECTOR& _position2, const float& _radius) const
{
	MV1_COLL_RESULT_POLY_DIM hit_poly;
	hit_poly.HitNum = 0;

	//壁ポリゴンがあるオブジェクトと当たり判定をとる
	for (auto&& iter : navimesh_handle_)
	{
		if (MV1SearchFrame((iter), "wall_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Capsule(
				(iter),
				MV1SearchFrame((iter), "wall_NavMesh"),
				_position1, _position2, _radius);
		}

		//当たり判定があれば抜ける
		if (hit_poly.HitNum > 0)
			return hit_poly;
	}
	return hit_poly;
}

MV1_COLL_RESULT_POLY Stage::GetHitLineToShootPoint(const VECTOR& _startline, const VECTOR& _endline) const
{
	MV1_COLL_RESULT_POLY hit_poly;

	hit_poly = MV1CollCheck_Line(
		shootpoint_,
		MV1SearchFrame(shootpoint_, "floor_NavMesh"),
		_startline, _endline);

	return hit_poly;
}

int Stage::GetHitFloorType(const VECTOR& _startline, const VECTOR& _endline) const
{
	MV1_COLL_RESULT_POLY hit_poly;
	hit_poly.HitFlag = 0;

	for (auto&& iter : navimesh_handle_)
	{
		//床ポリゴンがあるオブジェクトと当たり判定をとる
		if (MV1SearchFrame((iter), "floor_NavMesh") > 0)
		{
			hit_poly = MV1CollCheck_Line(
				(iter),
				MV1SearchFrame((iter), "floor_NavMesh"),
				_startline, _endline);
		}

		if (!hit_poly.HitFlag)
			continue;
		else
		{
			// キーの検索
			auto type = map_floortype_.find(iter);
			if (type != map_floortype_.end())
			{
				//登録済みなので終了
				return (*type).second;
			}
		}
	}
	return -1;
}