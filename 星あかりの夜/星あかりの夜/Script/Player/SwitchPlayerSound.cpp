/**
 * @file    SwitchPlayerSound.cpp
 * @brief  プレイヤーのサウンドをステータスから設定する処理
 *
 * @author Takuya Fujisawa
 * @date   2021/03/08
 */

#include "Player.h"
#include"../Stage/Stage.h"
using namespace starrynight::player;

void Player::SwitchPlayerSound()
{
	switch (status_)
	{
	case STATUS::WAIT:
		if (anim_play_time_ == 410)
		{
			se_.Load("Resource/sound/wait_voice.wav");
			se_.Play();
		}
		break;
	case STATUS::WALK:
		if (anim_play_time_ == 0 ||
			anim_play_time_ == 40)
		{
			switch (floor_type_)
			{
			case GRASS:
				se_.Load("Resource/sound/grass_walk.wav"); break;
			case ROCK:
				se_.Load("Resource/sound/rock_walk.wav"); break;
			case WOOD:
				se_.Load("Resource/sound/wood_walk.wav"); break;
			}
			se_.SetVolume_dB(-8.0f);
			se_.RandomPitch(0.3f);
			se_.Play();
		}
		break;
	case STATUS::RUN:
		if (anim_play_time_ == 8 ||
			anim_play_time_ == 30)
		{
			switch (floor_type_)
			{
			case GRASS:
				se_.Load("Resource/sound/grass_run.wav"); break;
			case ROCK:
				se_.Load("Resource/sound/rock_run.wav"); break;
			case WOOD:
				se_.Load("Resource/sound/wood_walk.wav"); break;
			}
			se_.SetVolume_dB(-8.0f);
			se_.RandomPitch(0.3f);
			se_.Play();
		}
		break;
	case STATUS::JUMP_START:
		if (anim_play_time_ == 0)
		{
			se_.Load("Resource/sound/jump.wav");
			se_.Play();
		}
		break;
	case STATUS::JUMP_END:
		if (anim_play_time_ != 0)
			break;

		if (jump_speed_ < -20)
		{
			se_.Load("Resource/sound/landing_voice.wav");
			se_.Play();
		}
		else
		{
			se_.Load("Resource/sound/shirimoti.wav");
			se_.SetVolume_dB(-12.0f);
			se_.Play();
		}
		break;
	case STATUS::SHOOT_END:
		if (anim_play_time_ == 0)
		{
			se_.Load("Resource/sound/shoot.wav");
			se_.Fade(0.0f, 1.0f);
			se_.Play();
		}
		break;
	case STATUS::DAMAGE:
		if (anim_play_time_ == 0)
		{
			se_.Load("Resource/sound/damage_voice1.wav");
			se_.Play();
		}
		if (anim_play_time_ == 110)
		{
			se_.Load("Resource/sound/shirimoti.wav");
			se_.Play();
		}
		if (anim_play_time_ == 180)
		{
			se_.Load("Resource/sound/damage_voice2.wav");
			se_.Play();
		}
		break;
	}
}