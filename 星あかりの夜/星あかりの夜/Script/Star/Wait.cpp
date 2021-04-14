/**
 * @file    StarAction.cpp
 * @brief  スターの待機処理
 *
 * @author Takuya Fujisawa
 * @date   2021/03/13
 */

#include"StageStarWaitState.h"
#include "StageStar.h"

using namespace starrynight::star;

void StageStarWaitState::Wait(Star& _star)
{
	VECTOR player_position = MV1GetPosition(resource::ResourceServer::GetModelHandle("player"));
	VECTOR position = _star.GetPosition();
	VECTOR rotation = _star.GetRotation();
	VECTOR distance = VSub(VAdd(player_position, VGet(0, 80, 0)), VAdd(position, VGet(0, 40, 0)));

	float length = VSize(distance);

	//プレイヤーとの距離が検出範囲であれば、飛び跳ねる
	if (length < detect_length_)
	{
		if (position.y <= _star.GetGroundPositionY())
		{
			position.y = _star.GetGroundPositionY();
			_star.SetJumpSpeed(_star.GetJumpHeight());

			_star.se_.Load("Resource/sound/star_wait.wav");
			//画面のスクリーン座標によって音の左右バランスを設定
			_star.se_.Pan(static_cast<int>(utility::GetScreenPosFromWorldPos(position).x));
			//プレイヤーとの距離で音の音量を設定
			float distance = 1.0f - VSize(VSub(MV1GetPosition(resource::ResourceServer::GetModelHandle("player")), position)) / detect_length_;
			distance = utility::clamp(distance, 0.01f, 1.0f);
			float volume = 20.0f * logf(distance);
			_star.se_.SetVolume_dB(volume);
			_star.se_.Play();
		}

		position.y += _star.GetJumpSpeed();
		_star.SetJumpSpeed(_star.GetJumpSpeed() - _star.GetGravity());
	}
	else
	{
		if (_star.GetJumpSpeed() >= -1.0f * _star.GetJumpHeight())
		{
			position.y += _star.GetJumpSpeed();
			_star.SetJumpSpeed(_star.GetJumpSpeed() - _star.GetGravity());
		}
		else
		{
			position.y = _star.GetGroundPositionY();
		}
	}

	rotation.y += DEG2RAD(_star.GetRotSpeed());

	//座標を更新
	_star.SetPosition(position);
	_star.SetRotation(rotation);
}