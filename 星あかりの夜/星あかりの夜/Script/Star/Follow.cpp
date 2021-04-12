/**
 * @file    StarAction.cpp
 * @brief  スターのプレイヤーについてくる処理
 *
 * @author Takuya Fujisawa
 * @date   2021/03/13
 */

#include "Star.h"
#include"../Player/Player.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::star;

void Star::Follow()
{
	mode::ModeGame* modegame = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	player_position = VAdd(player_position, VGet(0.0f, 50.0f, 0.0f));

	float move_min_size = 0.05f;
	//前フレームとの移動距離が小さい場合は、キューに追加しない
	if (VSize(VSub(player_position, old_player_position_)) > move_min_size)
		player_pos_history_.push(player_position);

	//プレイヤーとの距離が設定した間隔より大きい
	if (VSize(VSub(player_position, position_)) > star_num_ * follow_interval_)
	{
		VECTOR que_position = position_;

		while (!player_pos_history_.empty())
		{
			que_position = player_pos_history_.front();

			//プレイヤーとの距離が間隔より近ければ、遠くなるまで待つ
			if (VSize(VSub(player_position, que_position)) >= star_num_ * follow_interval_)
				player_pos_history_.pop();
			else
				break;
		}

		//星の移動
		VECTOR star_move = VScale(VSub(que_position, position_), move_min_size);
		position_ = VAdd(position_, star_move);

		//使用したプレイヤー座標を削除する
		if (!player_pos_history_.empty())
			player_pos_history_.pop();
	}

	old_player_position_ = player_position;

	//プレイヤーがダメージを受けたかどうかを取得
	bool player_damaged = false;
	for (auto iter = modegame->object_server_.List()->begin(); iter != modegame->object_server_.List()->end(); iter++)
	{
		if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(*iter);
			player_damaged = player->GetDamageFlag();
			break;
		}
	}

	//プレイヤーのスター所持数を初期化し、拡散処理へ遷移
	if (player_damaged)
	{
		status_ = STATUS::DIFFUSION;
		star_num_ = 0;
		jump_speed_ = jump_height_ * 3.0f;

		//queueを初期化
		std::queue<VECTOR> empty;
		std::swap(player_pos_history_, empty);
	}

	rotation_.y += DEG2RAD(rot_speed_);
}