/**
 * @file    StarAction.cpp
 * @brief  スターのプレイヤーについてくる処理
 *
 * @author Takuya Fujisawa
 * @date   2021/03/13
 */

#include "StageStarFollowState.h"
#include "StageStar.h"

using namespace starrynight::star;

void StageStarFollowState::Follow(Star& _star)
{
	VECTOR position = _star.GetPosition();
	VECTOR rotation = _star.GetRotation();

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	player_position = VAdd(player_position, VGet(0.0f, 50.0f, 0.0f));

	float move_min_size = 0.05f;
	//前フレームとの移動距離が小さい場合は、キューに追加しない
	if (VSize(VSub(player_position, old_player_position_)) > move_min_size)
		player_pos_history_.push(player_position);

	//プレイヤーとの距離が設定した間隔より大きい
	if (VSize(VSub(player_position, position)) > _star.GetPlayersStarNum() * follow_interval_)
	{
		VECTOR que_position = position;

		while (!player_pos_history_.empty())
		{
			que_position = player_pos_history_.front();

			//プレイヤーとの距離が間隔より近ければ、遠くなるまで待つ
			if (VSize(VSub(player_position, que_position)) >= _star.GetPlayersStarNum() * follow_interval_)
				player_pos_history_.pop();
			else
				break;
		}

		//星の移動
		VECTOR star_move = VScale(VSub(que_position, position), move_min_size);
		position = VAdd(position, star_move);

		//使用したプレイヤー座標を削除する
		if (!player_pos_history_.empty())
			player_pos_history_.pop();
	}

	old_player_position_ = player_position;

	rotation.y += DEG2RAD(_star.GetRotSpeed());

	//座標を更新
	_star.SetPosition(position);
	_star.SetRotation(rotation);
}