/**
 * @file    EnemyWait.cpp
 * @brief  敵の待機状態処理
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#include"EnemyWaitState.h"
#include"Enemy.h"
#include"../Mode/ModeGame.h"

using namespace starrynight::enemy;

void EnemyWaitState::Wait(Enemy& _enemy)
{
	mode::ModeGame* mode_game = mode::ModeGame::GetModeGame();
	int elapsed_frame = mode_game->GetModeCount() - waitstart_frame_;

	//索敵状態であれば、経過時間によって待機と直進を交互に繰り返す
	float random_rot_direction = 1.0f;
	if (elapsed_frame % (60 * (GetRand(5) + 1)) == 0)
	{
		if (_enemy.GetMoveStatus() == enemy::Enemy::MOVE_STATUS::WAIT)
		{
			_enemy.SetMoveStatus(enemy::Enemy::MOVE_STATUS::STRAIGHT);

			//回転方向をランダムに変える
			random_rot_direction *= powf(-1.0f, static_cast<float>(GetRand(1) + 1));
		}
		else
		{
			_enemy.SetMoveStatus(enemy::Enemy::MOVE_STATUS::WAIT);
		}
	}

	//状態によって移動する方向や量を決定する
	VECTOR move = { 0,0,0 };
	VECTOR rotation = _enemy.GetRotation();

	switch (_enemy.GetMoveStatus())
	{
	case Enemy::MOVE_STATUS::WAIT:
		_enemy.SetAnimStatus(Enemy::ANIM_STATUS::WAIT);

		rotation.y += DEG2RAD(_enemy.GetRotationSpeed()) * random_rot_direction;
		break;
	case Enemy::MOVE_STATUS::STRAIGHT:
		_enemy.SetAnimStatus(Enemy::ANIM_STATUS::MOVE);

		move = VScale(VNorm(utility::GetForwardVector(rotation.y)), _enemy.GetWalkSpeed());
		break;
	}

	_enemy.SetRotation(rotation);
	_enemy.Move(move);
}