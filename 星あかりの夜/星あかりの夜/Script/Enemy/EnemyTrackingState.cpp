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
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();
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
	std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();

	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR enemy_position = _enemy.GetPosition();
	float player_distance = VSize(VSub(player_position, enemy_position));
	float player_distance_y = abs(player_position.y - enemy_position.y);
	float detect_y_length = 200.0f;

	//プレイヤーがダメージを受けたかどうか取得
	bool player_damaged = false;
	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		if ((iter)->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			std::shared_ptr<player::Player> player = std::dynamic_pointer_cast<player::Player>(iter);
			player_damaged = player->GetDamageFlag();
			break;
		}
	}

	//追跡範囲から出たかを確認
	bool out_of_tracking = false;
	if (player_distance > _enemy.GetDetectLength() ||
		player_distance_y > detect_y_length)
	{
		out_of_tracking = true;
	}

	//検出範囲からプレイヤーが出たか、プレイヤーがダメージ中の場合
	if (out_of_tracking == true ||
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
	Tracking(_enemy);
}