/**
 * @file    EnemyAttackState.h
 * @brief  敵の攻撃処理クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/12
 */

#include "EnemyAttackState.h"
#include"EnemyEscapeState.h"
#include"../Player/Player.h"
#include"../Effect/DamageEffect.h"
#include"../Mode/ModeGame.h"
#include"Enemy.h"
using namespace starrynight::enemy;

EnemyAttackState::EnemyAttackState()
{
}

EnemyAttackState::~EnemyAttackState()
{
}

void EnemyAttackState::Enter(Enemy& _enemy)
{
	_enemy.SetMoveStatus(Enemy::MOVE_STATUS::ATTACK);

	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();

	VECTOR position = _enemy.GetPosition();
	VECTOR rotation = _enemy.GetRotation();
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));

	for (auto&& iter : (*mode_game->object_server_.List()))
	{
		//プレイヤーをエネミーの方に向ける
		if (iter->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			player::Player* player = static_cast<player::Player*>(iter);
			player->SetRotation(VGet(0.0f, rotation.y + DEG2RAD(180.0f), 0.0f));
			player->SetDamageFlag(true);

			//プレイヤーが星を所持していない場合、HPを減らす
			if (mode_game->GetPlayerStarNum() == 0)
				player->AddPlayerHP(-1);

			break;
		}
	}
	//プレイヤーのダメージエフェクトを描画
	effect::DamageEffect* damage_effect = NEW effect::DamageEffect();
	VECTOR damage_pos = VScale(VAdd(player_position, position), 0.5f);
	damage_pos = VAdd(damage_pos, VGet(0, 50, 0));

	damage_effect->SetPosition(damage_pos);
	damage_effect->SetRotation(VGet(0, 0, 0));
	damage_effect->Initialize();

	damage_effect->PlayEffect();
	mode_game->effect_server_.Add(damage_effect);

	_enemy.se_.Load("Resource/sound/enemy_se2.wav");
	_enemy.se_.Play();
	_enemy.se_.Fade(0.0f, 1.5f);
}

void EnemyAttackState::Exit(Enemy& _enemy)
{
}

void EnemyAttackState::Input(Enemy& _enemy)
{
	//ダメージモーションの再生が完了したら
	if (_enemy.GetAnimPlayTime() >= _enemy.GetAnimTotalTime())
	{
		//プレイヤーから離れていく状態へ遷移
		_enemy.ChangeEnemyState("Escape");
	}

	//ダメージモーション中は攻撃を継続
	_enemy.SetAnimStatus(Enemy::ANIM_STATUS::ATTACK);
}

void EnemyAttackState::Update(Enemy& _enemy)
{
	VECTOR move = { 0,0,0 };
	_enemy.Move(move);
}