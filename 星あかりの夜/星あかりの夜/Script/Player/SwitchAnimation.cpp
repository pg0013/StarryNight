/**
 * @file    SwitchAnimation.cpp
 * @brief  アニメーション切り替え関数
 *
 * @author Takuya Fujisawa
 * @date   2021/01/12
 */
#include"Player.h"
#include"../Mode/ModeGame.h"
using namespace starrynight::player;

void Player::SwitchAnimation(STATUS _old_status)
{
	if (_old_status == status_)
	{
		//再生時間を進める
		anim_play_time_ += 1.0f;
		anim_rate_ += 0.1f;
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
		case STATUS::WAIT:
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "kyrie_wait.x"), -1, FALSE);
			break;
		case STATUS::WALK:
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "kyrie_run"), -1, FALSE);
			break;
		case STATUS::JUMP:
			//TODO:仮モーション
			anim_attach_index_ = MV1AttachAnim(handle_, MV1GetAnimIndex(handle_, "kyrie_run"), -1, FALSE);
			break;
		}

		//アニメーションの総再生時間を取得し、再生時間を初期化
		anim_total_time_ = MV1GetAttachAnimTotalTime(handle_, anim_attach_index_);
		anim_play_time_ = 0.0f;

		anim_rate_ = 0.0f;
	}

	//再生時間をループする
	if (anim_play_time_ > anim_total_time_)
		anim_play_time_ = 0.0f;

	//アニメーションブレンドの終了処理
	if (anim_rate_ >= 1.0f)
	{
		anim_rate_ = 1.0f;
		MV1DetachAnim(handle_, old_anim_attach_index_);
		old_anim_attach_index_ = -1;
	}
}