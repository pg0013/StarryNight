/**
 * @file    SwitchAnimation.cpp
 * @brief  エネミーのアニメーション管理処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include "Enemy.h"
#include"../Mode/ModeGame.h"
#include"../Stage/Stage.h"

using namespace starrynight::enemy;

void Enemy::SwitchEnemyAnimation(ANIM_STATUS _old_status)
{
	if (_old_status == anim_status_)
	{
		if (anim_loop_flag_ == false &&
			anim_play_time_ > anim_total_time_)
		{
			//ループしないので、再生時間を進めない
			anim_play_time_ = anim_total_time_;
		}
		else
		{
			//再生時間を進める
			if (anim_status_ == ANIM_STATUS::ATTACK)
				anim_play_time_ += 2.0f;
			else
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
		switch (anim_status_)
		{
		case ANIM_STATUS::MOVE:
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "walk"), -1, FALSE);
			anim_loop_flag_ = true;
			break;
		case ANIM_STATUS::WAIT:
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "wait"), -1, FALSE);
			anim_loop_flag_ = true;
			break;
		case ANIM_STATUS::RUN:
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "run"), -1, FALSE);
			anim_loop_flag_ = true;
			break;
		case ANIM_STATUS::RUSH:
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "rush"), -1, FALSE);
			anim_loop_flag_ = true;
			break;
		case ANIM_STATUS::ATTACK:
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