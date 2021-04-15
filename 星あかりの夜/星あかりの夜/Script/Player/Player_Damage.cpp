/**
 * @file    Player_Damage.cpp
 * @brief  プレイヤーのダメージ処理
 *
 * @author Takuya Fujisawa
 * @date   2021/04/15
 */

#include"PlayerDamageState.h"
#include"Player.h"
#include"../Stage/Stage.h"

using namespace starrynight::player;

void PlayerDamageState::Damage(Player& _player)
{
	VECTOR position = _player.GetPosition();

	//腰から地面までの線分ベクトル
	VECTOR start_line = VAdd(position, VGet(0, 40.0f, 0));
	VECTOR end_line = VAdd(position, VGet(0, -5.0f, 0));

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;
	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

	//ジャンプ中にダメージを受けた時には地面に着地させる
	if (!hit_poly_floor.HitFlag)
	{
		position.y += jump_speed_;

		//腰から地面までの線分ベクトル
		VECTOR start_line = VAdd(position, VGet(0, 40.0f, 0));
		VECTOR end_line = VAdd(position, VGet(0, -5.0f, 0));

		//Navimeshとの当たり判定
		MV1_COLL_RESULT_POLY hit_poly_floor;
		hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);

		//ジャンプ開始処理
		if (hit_poly_floor.HitFlag)
		{
			position.y = hit_poly_floor.HitPosition.y;
		}
		//ジャンプ加速処理
		jump_speed_ -= gravity_;
	}

	//位置を設定
	_player.SetPosition(position);
}