/**
 * @file    EnemyTrackingState.cpp
 * @brief  敵の追跡状態クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/12
 */

#include"EnemyTrackingState.h"
#include"EnemyWaitState.h"
#include"EnemyAttackState.h"
#include"../Player/Player.h"
#include"Enemy.h"
#include"../Mode/ModeGame.h"
using namespace starrynight::enemy;

EnemyTrackingState::EnemyTrackingState()
{
	attack_length_ = 70.0f;
	track_start_frame_ = 0;
}

EnemyTrackingState::~EnemyTrackingState()
{
}

void EnemyTrackingState::Enter(Enemy& _enemy)
{
	//追跡開始フレームを記録
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();
	track_start_frame_ = mode_game->GetModeCount();

	VECTOR position = _enemy.GetPosition();

	//プレイヤー発見SEを再生
	_enemy.attention_se_.Load("Resource/sound/enemy_se3.wav");
	//スクリーン上の位置に対して、左右バランスを設定
	_enemy.attention_se_.Pan(static_cast<int>(utility::GetScreenPosFromWorldPos(position).x));
	_enemy.attention_se_.Play();
}

void EnemyTrackingState::Exit(Enemy& _enemy)
{
}

void EnemyTrackingState::Input(Enemy& _enemy)
{
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR enemy_position = _enemy.GetPosition();
	float player_distance = VSize(VSub(player_position, enemy_position));
	float player_distance_y = abs(player_position.y - enemy_position.y);

	//プレイヤーがダメージを受けたかどうか取得
	bool player_damaged = false;
	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(iter);
			player_damaged = player->GetDamageFlag();
			break;
		}
	}

	//検出範囲からプレイヤーが出たか、プレイヤーがダメージ中の場合
	if (player_distance > _enemy.GetDetectLength() ||
		player_damaged == true)
	{
		//待機状態へ移行
		_enemy.ChangeEnemyState("Wait");
	}

	if (player_distance <= attack_length_)
	{
		//攻撃状態へ移行
		_enemy.ChangeEnemyState("Attack");
	}

	//追跡状態を継続
	_enemy.SetAnimStatus(Enemy::ANIM_STATUS::RUN);
	_enemy.SetMoveStatus(Enemy::MOVE_STATUS::TRACKING);
}

void EnemyTrackingState::Update(Enemy& _enemy)
{
	VECTOR move = { 0,0,0 };

	//プレイヤーを発見してから、ワンモーションは移動しない
	int stop_frame = 25;
	if (::mode::ModeServer::GetInstance()->Get("Game")->GetModeCount() - track_start_frame_ < stop_frame)
	{
		//移動しないでその場で足踏み
		move = { 0,0,0 };
		_enemy.Move(move);

		return;
	}

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR position = _enemy.GetPosition();
	VECTOR rotation = _enemy.GetRotation();

	float player_distance = VSize(VSub(player_position, position));

	VECTOR enemy_to_player = VSub(player_position, position);
	VECTOR forward = utility::GetForwardVector(rotation.y);

	VECTOR cross = VCross(forward, enemy_to_player);

	//プレイヤーがエネミーに対して左右どちらかにいるか判定する
	if (VCross(VNorm(forward), VNorm(enemy_to_player)).y >= 0)
	{
		rotation.y += DEG2RAD(_enemy.GetRotationSpeed()) * VSize(VCross(forward, enemy_to_player)) * 0.02f;
	}
	else
	{
		rotation.y -= DEG2RAD(_enemy.GetRotationSpeed()) * VSize(VCross(forward, enemy_to_player)) * 0.02f;
	}

	move = VScale(VNorm(utility::GetForwardVector(rotation.y)), _enemy.GetRunSpeed());

	_enemy.SetRotation(rotation);
	_enemy.Move(move);
}