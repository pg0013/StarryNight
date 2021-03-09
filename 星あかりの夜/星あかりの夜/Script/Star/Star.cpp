/**
 * @file    Star.cpp
 * @brief  ステージ内のスター挙動実装クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/01/23
 */

#include "Star.h"
#include"../Stage/Stage.h"
#include"../Player/Player.h"
#include"../Effect/GetStarEffect.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::star;

Star::Star()
{
	status_ = STATUS::WAIT;
	rot_speed_ = 5.0f;
	jump_height_ = 5.0f;
	jump_speed_ = -jump_height_;
	gravity_ = 0.1f;
	star_num_ = 0;
	follow_interval_ = 100.0f;
	old_player_position_ = { 0,0,0 };
	ground_position_y_ = 0.0f;
}

Star::~Star()
{
	if (se_.CheckIsRunning())
	{
		se_.Pause();
		se_.Destroy();
	}
}

void Star::Initialize()
{
	position_ = MV1GetPosition(handle_);
	rotation_ = MV1GetRotationXYZ(handle_);
	ground_position_y_ = position_.y;
}

void Star::Process()
{
	switch (status_)
	{
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
	float detect_length = 500.0f;//検出範囲
	float touch_length = 100.0f;//取得範囲

	//プレイヤーとの距離が取得範囲ないであれば、followモードへ遷移
	if (length < touch_length)
	{
		status_ = STATUS::FOLLOW;

		//取得したスターの数を増やす
		mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
		mode_game->AddPlayerStarNum();
		star_num_ = mode_game->GetPlayerStarNum();

		VECTOR player_rotation = MV1GetRotationXYZ(resource::ResourceServer::GetModelHandle("player"));
		player_position = VAdd(player_position, VGet(0.0f, 50.0f, 0.0f));
		position_ = VAdd(player_position, VScale(VNorm(utility::GetForwardVector(player_rotation.y)), -0.5f * follow_interval_));

		//星獲得エフェクトを生成
		effect::GetStarEffect* getstar_effect = NEW effect::GetStarEffect();
		getstar_effect->SetPosition(player_position);
		getstar_effect->Initialize();
		getstar_effect->PlayEffect();
		mode_game->effect_server_.Add(getstar_effect);

		se_.Load("Resource/sound/getstar.wav");
		se_.Play();
	}

	//プレイヤーとの距離が検出範囲であれば、飛び跳ねる
	if (length < detect_length)
	{
		if (position_.y < ground_position_y_)
		{
			position_.y = ground_position_y_;
			jump_speed_ = jump_height_;
		}

		position_.y += jump_speed_;
		jump_speed_ -= gravity_;
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

	rotation_.y += DEG2RAD(rot_speed_);
}

void Star::Follow()
{
	mode::ModeGame* modegame = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	player_position = VAdd(player_position, VGet(0.0f, 50.0f, 0.0f));

	//前フレームとの移動距離が小さい場合は、キューに追加しない
	float move_min_size = 1.0f;
	if (VSize(VSub(player_position, old_player_position_)) > move_min_size)
		player_pos_history_.push(player_position);

	//プレイヤーとの距離が設定した間隔より大きい
	if (VSize(VSub(player_position, position_)) > star_num_ * follow_interval_)
	{
		VECTOR que_position;

		while (!player_pos_history_.empty())
		{
			que_position = player_pos_history_.front();

			//プレイヤーとの距離が間隔より近ければ、遠くなるまで待つ
			if (VSize(VSub(player_position, que_position)) >= star_num_ * follow_interval_)
				player_pos_history_.pop();
			else
				break;
		}

		//if (player_position.y <= 0)
		//{
		//	ground_position_y_ = position_.y;
		//	modegame->SetPlayerStarNum(0);

		//	//queueを初期化
		//	std::queue<VECTOR> empty;
		//	std::swap(player_pos_history_, empty);

		//	//待機状態にする
		//	status_ = STATUS::WAIT;
		//	return;
		//}

		VECTOR star_move = VScale(VSub(que_position, position_), 0.1f);
		position_ = VAdd(position_, star_move);

		if (!player_pos_history_.empty())
			player_pos_history_.pop();
	}

	old_player_position_ = player_position;

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
	if (player_damaged)
	{
		status_ = STATUS::DIFFUSION;
		star_num_ = 0;
		jump_speed_ = jump_height_ * 3.0f;

		modegame->SetPlayerStarNum(0);

		//queueを初期化
		std::queue<VECTOR> empty;
		std::swap(player_pos_history_, empty);
	}

	rotation_.y += DEG2RAD(rot_speed_);
}

void Star::Diffusion()
{
	VECTOR move = VNorm(utility::GetForwardVector(rotation_.y));
	move = VScale(move, 10.0f);

	//ステージ範囲外まで到達したら、移動しない
	float out_of_stage = 2300.0f;
	if (VSize(VAdd(position_, move)) > out_of_stage)
		move = VAdd(move, VScale(VNorm(VSub(VGet(0, 0, 0), position_)),50.0f));

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

int Star::IsStageStarDraw()
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	bool stage_draw_flag = true;

	for (auto iter = mode_game->object_server_.List()->begin(); iter != mode_game->object_server_.List()->end(); iter++)
	{
		if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(*iter);
			stage_draw_flag = player->GetPlayerSlingShotStatus();
			break;
		}
	}

	if (stage_draw_flag)
		return FALSE;
	else
		return TRUE;
}

void Star::Render()
{
	MV1SetPosition(handle_, position_);
	MV1SetRotationXYZ(handle_, rotation_);

	SetUseLighting(FALSE);
	MV1SetVisible(handle_, IsStageStarDraw());
	MV1DrawModel(handle_);
	SetUseLighting(TRUE);

#ifdef DEBUG_FUNCTION
	DrawLine3D(position_, VAdd(position_, VGet(0, 2000, 0)), GetColor(255, 255, 0));
#endif // debug_
}