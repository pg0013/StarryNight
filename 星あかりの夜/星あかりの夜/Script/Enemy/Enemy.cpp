/**
 * @file    Enemy.h
 * @brief  敵オブジェクトクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/20
 */

#include "Enemy.h"
#include"../Player/Player.h"
#include"../Mode/ModeGame.h"
#include"../Stage/Stage.h"

using namespace starrynight::enemy;

Enemy::Enemy()
{
	enemy_param_.LoadModelParameters("enemy");
	std::vector<std::string> param = { "walk_speed","run_speed","rot_speed","detect_length" };
	enemy_param_.LoadEnemyParameters(param);

	handle_ = resource::ResourceServer::GetModelHandle("enemy");
	position_ = MV1GetPosition(handle_);
	rotation_ = MV1GetRotationXYZ(handle_);

	walk_speed_ = enemy_param_.GetEnemyParam("walk_speed");
	run_speed_ = enemy_param_.GetEnemyParam("run_speed");
	rot_speed_ = enemy_param_.GetEnemyParam("rot_speed");
	detect_length_ = enemy_param_.GetEnemyParam("detect_length");

	attacked_flag_ = false;
	random_rot_direction_ = 1.0f;

	status_ = STATUS::NONE;
	move_status_ = MOVE_STATUS::WAIT;
	anim_attach_index_ = -1;
	old_anim_attach_index_ = anim_attach_index_;
	anim_rate_ = 1.0f;
	anim_loop_flag_ = true;

	Initialize();
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	start_frame_ = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount();
}

void Enemy::Process()
{
	STATUS old_status = status_;
	status_ = STATUS::WAIT;
	VECTOR move = Wander();
	Move(move);
	SwitchAnimation(old_status);
}

VECTOR Enemy::Wander()
{
	mode::ModeGame* mode_game = static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));
	int elapsed_frame = mode_game->GetModeCount() - start_frame_;

	MOVE_STATUS  old_status = move_status_;

	if (elapsed_frame % (60 * (GetRand(5) + 1)) == 0)
	{
		if (old_status == MOVE_STATUS::STRAIGHT)
		{
			move_status_ = MOVE_STATUS::WAIT;
			random_rot_direction_ *= powf(-1.0f, static_cast<float>(GetRand(1) + 1));
		}
		if (old_status == MOVE_STATUS::WAIT)
		{
			move_status_ = MOVE_STATUS::STRAIGHT;
		}
	}

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	float player_distance = VSize(VSub(player_position, position_));

	if (player_distance < detect_length_ &&
		move_status_ != MOVE_STATUS::ESCAPE)
	{
		move_status_ = MOVE_STATUS::TRACKING;
	}

	if (player_distance <= 70.0f &&
		move_status_ != MOVE_STATUS::ESCAPE)
	{
		move_status_ = MOVE_STATUS::ATTACK;
		if (attacked_flag_ == false)
		{
			for (auto iter = mode_game->object_server_.List()->begin(); iter != mode_game->object_server_.List()->end(); iter++)
			{
				//プレイヤーをエネミーの方に向ける
				if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
				{
					player::Player* player = static_cast<player::Player*>(*iter);
					player->SetRotation(VGet(0.0f, rotation_.y + DEG2RAD(180.0f), 0.0f));
					player->SetDamageFlag(true);
					break;
				}
			}
			attacked_flag_ = true;
		}
	}

	VECTOR move = { 0,0,0 };

	//clsDx();
	switch (move_status_)
	{
	case MOVE_STATUS::WAIT:
		status_ = STATUS::WAIT;
		//printfDx("WAIT\n");
		rotation_.y += DEG2RAD(rot_speed_) * random_rot_direction_;
		break;
	case MOVE_STATUS::STRAIGHT:
		//printfDx("STRAIGHT\n");

		status_ = STATUS::MOVE;
		move = VScale(VNorm(utility::GetForwardVector(rotation_.y)), walk_speed_);
		break;
	case MOVE_STATUS::TRACKING:
		//printfDx("TRACKING\n");

		status_ = STATUS::RUN;

		VECTOR enemy_to_player = VSub(player_position, position_);
		VECTOR forward = utility::GetForwardVector(rotation_.y);

		VECTOR cross = VCross(forward, enemy_to_player);

		if (VCross(VNorm(forward), VNorm(enemy_to_player)).y >= 0)
		{
			rotation_.y += DEG2RAD(rot_speed_) * VSize(VCross(forward, enemy_to_player)) * 0.01f;
		}
		else
		{
			rotation_.y -= DEG2RAD(rot_speed_) * VSize(VCross(forward, enemy_to_player)) * 0.01f;
		}

		move = VScale(VNorm(utility::GetForwardVector(rotation_.y)), run_speed_);

		if (player_distance > detect_length_)
		{
			move_status_ = MOVE_STATUS::WAIT;
			move = { 0,0,0 };
		}
		break;
	case MOVE_STATUS::ATTACK:
		//printfDx("ATTACK\n");
		status_ = STATUS::ATTACK;

		if (anim_play_time_ > 40.0f)
		{
			for (auto iter = mode_game->object_server_.List()->begin(); iter != mode_game->object_server_.List()->end(); iter++)
			{
				if ((*iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
				{
					player::Player* player = static_cast<player::Player*>(*iter);
					player->SetDamageAnimFlag(true);
					break;
				}
			}
		}

		if (anim_play_time_ >= anim_total_time_)
		{
			rotation_.y -= DEG2RAD(180.0f);
			move_status_ = MOVE_STATUS::ESCAPE;
			status_ = STATUS::MOVE;
			attacked_flag_ = false;
			move = VScale(VNorm(utility::GetForwardVector(rotation_.y)), run_speed_);
		}
		break;
	case MOVE_STATUS::ESCAPE:
		//printfDx("ESCAPE\n");

		status_ = STATUS::MOVE;

		if (player_distance > detect_length_ + 100.0f)
			move_status_ = MOVE_STATUS::WAIT;
		else
			move = VScale(VNorm(utility::GetForwardVector(rotation_.y)), run_speed_);

		break;
	}
	return move;
}

void Enemy::Move(VECTOR _move)
{
	VECTOR old_position = position_;
	position_ = VAdd(position_, _move);

	VECTOR sphere_positon = VAdd(position_, VGet(0, 45, 0));
	float radius = 35.0f;

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

		VECTOR escape = VCross(_move, normal);
		escape = VCross(normal, escape);
		escape.y = 0.0f;

		position_ = old_position;
		position_ = VAdd(position_, escape);

		MV1CollResultPolyDimTerminate(hit_poly_wall);

		//押し出し処理
		while (1)
		{
			VECTOR shpere_positon = VAdd(position_, VGet(0, 45, 0));
			float radius = 35.0f;

			MV1_COLL_RESULT_POLY_DIM hit_poly_wallpush;
			hit_poly_wallpush = stage::Stage::GetInstance()->GetHitSphereToWall(shpere_positon, radius);

			if (hit_poly_wallpush.HitNum == 0)
				break;

			VECTOR normal = { 0,0,0 };
			for (int i = 0; i < hit_poly_wall.HitNum; i++)
			{
				normal = VAdd(normal, VNorm(hit_poly_wall.Dim[i].Normal));
			}
			normal = VNorm(normal);

			position_ = VAdd(position_, VScale(normal, 0.5f));

			MV1CollResultPolyDimTerminate(hit_poly_wallpush);
		}
	}

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_floor;

	//腰から地面までの線分ベクトル
	VECTOR start_line = VAdd(position_, VGet(0, 10.0f, 0));
	VECTOR end_line = VAdd(position_, VGet(0, -10.0f, 0));

	hit_poly_floor = stage::Stage::GetInstance()->GetHitLineToFloor(start_line, end_line);
	if (hit_poly_floor.HitFlag)
		position_.y = hit_poly_floor.HitPosition.y;
	else
	{
		position_ = old_position;
		status_ = STATUS::WAIT;
	}
}

void Enemy::SwitchAnimation(STATUS _old_status)
{
	if (_old_status == status_)
	{
		if (anim_loop_flag_ == false &&
			anim_play_time_ > anim_total_time_)
		{
			anim_play_time_ = anim_total_time_;
		}
		else
		{
			//再生時間を進める
			anim_play_time_ += 1.0f;
			anim_rate_ += 0.2f;
		}
	}
	else
	{
		//すでにアニメーションがアタッチされていたらデタッチする
		if (anim_attach_index_ != -1)
		{
			MV1DetachAnim(handle_, old_anim_attach_index_);
			//遷移元のアニメーションを保存する
			old_anim_attach_index_ = anim_attach_index_;
		}

		//ステータスに合わせてアニメーションを変更
		switch (status_)
		{
		case STATUS::MOVE:
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "walk"), -1, FALSE);
			anim_loop_flag_ = true;
			break;
		case STATUS::WAIT:
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "wait"), -1, FALSE);
			anim_loop_flag_ = true;
			break;
		case STATUS::RUN:
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "run"), -1, FALSE);
			anim_loop_flag_ = true;
			break;
		case STATUS::RUSH:
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "rush"), -1, FALSE);
			anim_loop_flag_ = true;
			break;
		case STATUS::ATTACK:
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "attack"), -1, FALSE);
			anim_loop_flag_ = false;
			break;
		}

		//アニメーションの総再生時間を取得し、再生時間を初期化
		anim_total_time_ = MV1GetAttachAnimTotalTime(handle_, anim_attach_index_);
		anim_play_time_ = 0.0f;

		anim_rate_ = 0.0f;
	}

	//再生時間をループする
	if (anim_loop_flag_ &&
		anim_play_time_ > anim_total_time_)
		anim_play_time_ = 0.0f;

	//アニメーションブレンドの終了処理
	if (anim_rate_ >= 1.0f)
	{
		anim_rate_ = 1.0f;
		MV1DetachAnim(handle_, old_anim_attach_index_);
		old_anim_attach_index_ = -1;
	}
}

void Enemy::Render()
{
	if (anim_rate_ >= 1.0f)
		MV1SetAttachAnimTime(handle_, anim_attach_index_, anim_play_time_);
	else
	{
		MV1SetAttachAnimBlendRate(handle_, old_anim_attach_index_, 1.0f - anim_rate_);
		MV1SetAttachAnimBlendRate(handle_, anim_attach_index_, anim_rate_);
	}

	MV1SetPosition(handle_, position_);
	MV1SetRotationXYZ(handle_, rotation_);
	MV1DrawModel(handle_);

	VECTOR start = VAdd(position_, VGet(0, 50.0f, 0));
	VECTOR end = VAdd(start, VScale(VNorm(utility::GetForwardVector(rotation_.y)), 150.0f));
	DrawLine3D(start, end, DEBUG_COLOR);
}