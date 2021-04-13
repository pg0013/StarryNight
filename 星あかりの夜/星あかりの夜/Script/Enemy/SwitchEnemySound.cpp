/**
 * @file    SwitchEnemySound.cpp
 * @brief  SEの切り替えを行う処理
 *
 * @author Takuya Fujisawa
 * @date    202103/10
 */

#include "Enemy.h"

using namespace starrynight::enemy;

void Enemy::SwitchEnemySound()
{
	//ステータスに合わせてアニメーションを変更
	switch (anim_status_)
	{
	case ANIM_STATUS::RUN:
		if (anim_play_time_ == 0 ||
			anim_play_time_ == 25)
		{
			se_.Load("Resource/sound/enemy_se1.wav");

			se_.RandomPitch(0.3f);
			//スクリーン上の位置に対して、左右バランスを設定
			se_.Pan(static_cast<int>(utility::GetScreenPosFromWorldPos(position_).x));

			//プレイヤーとの距離に対して、音量を設定
			float distance = 1.0f - VSize(VSub(MV1GetPosition(resource::ResourceServer::GetModelHandle("player")), position_)) / detect_length_;
			//敵の足音は遠くでも少し聞こえるぐらいにするため、最小値を-12dBに設定
			distance = utility::clamp(distance, 0.25f, 1.0f);
			float volume = 20.0f * logf(distance);

			se_.SetVolume_dB(volume);
			se_.Play();
		}
		break;
	}
}