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

using namespace starrynight::enemy;

Enemy::Enemy(std::string _handle_name)
{
	//エネミーのパラメータをjsonから読み込み
	std::vector<std::string> param = { "walk_speed","run_speed","rot_speed","detect_length" };
	enemy_param_.LoadEnemyParameters(param);

	//配置情報を初期化
	handle_ = resource::ResourceServer::GetModelHandle(_handle_name);
	position_ = MV1GetPosition(handle_);
	rotation_ = MV1GetRotationXYZ(handle_);

	//パラメータを初期化
	walk_speed_ = enemy_param_.GetEnemyParam("walk_speed");
	run_speed_ = enemy_param_.GetEnemyParam("run_speed");
	rot_speed_ = enemy_param_.GetEnemyParam("rot_speed");
	detect_length_ = enemy_param_.GetEnemyParam("detect_length");

	attacked_flag_ = false;
	random_rot_direction_ = 1.0f;
	attack_length_ = 70.0f;
	track_start_frame_ = 0;

	//状態遷移用変数を初期化
	anim_status_ = ANIM_STATUS::WAIT;
	move_status_ = MOVE_STATUS::WAIT;

	//アニメーション情報を初期化
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
	//生成されたときのフレームを記録
	start_frame_ = ::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount();
}

void Enemy::Process()
{
	ANIM_STATUS old_anim_status = anim_status_;

	//状態遷移を行う
	SwitchStatus(old_anim_status);

	//ステータスによって移動量を決定し、移動
	VECTOR move = DecideMoveAmount();
	Move(move);

	//アニメーション切り替えを行う
	SwitchEnemyAnimation(old_anim_status);
	//サウンドの切り替えを行う
	SwitchEnemySound();
}

void Enemy::Render()
{
	//アニメーションブレンドを行う
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

#ifdef DEBUG_FUNCTION
	VECTOR start = VAdd(position_, VGet(0, 50.0f, 0));
	VECTOR end = VAdd(start, VScale(VNorm(utility::GetForwardVector(rotation_.y)), 150.0f));
	DrawLine3D(start, end, DEBUG_COLOR);
#endif
}