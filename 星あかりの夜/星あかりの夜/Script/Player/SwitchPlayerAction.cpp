/**
 * @file    SwitchPlayerAction.cpp
 * @brief  プレイヤーのアクション切り替えを行う処理
 *
 * @author Takuya Fujisawa
 * @date   2021/02/24
 */

#include"Player.h"
#include"../Stage/Stage.h"
#include"../Mode/ModeGame.h"
using namespace starrynight::player;

void Player::SwitchPlayerAction()
{
	if (damage_flag_)
	{
		Damage();
		return;
	}

	XINPUT_STATE x_input = appframe::ApplicationBase::GetInstance()->GetXInputState();

	int key = appframe::ApplicationBase::GetInstance()->GetKey();

	mode::ModeGame* mode_game =
		static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game"));

	//Navimeshとの当たり判定
	MV1_COLL_RESULT_POLY hit_poly_shootpoint;

	//腰から地面までの線分ベクトル
	VECTOR start_line = VAdd(position_, VGet(0, 40.0f, 0));
	VECTOR end_line = VAdd(position_, VGet(0, -30.0f, 0));

	hit_poly_shootpoint = stage::Stage::GetInstance()->GetHitLineToShootPoint(start_line, end_line);

	if (x_input.RightTrigger == 255 &&
		jump_flag_ == false &&
		mode_game->GetPlayerStarNum() > 0 &&
		hit_poly_shootpoint.HitFlag)
		slingshot_flag_ = true;

	if (camera::Camera::GetInstance()->GetStatus() == camera::Camera::STATUS::SKYSTAR)
	{
		return;
	}

	if (gameover_flag_)
		return;

	if (slingshot_flag_)
	{
		//射撃用のカメラに設定
		camera::Camera::GetInstance()->SetStatus(camera::Camera::STATUS::SHOOT);
		mode_game->ui_.SetDrawPlayerUIFlag(false);
		//射撃アクションを行う
		HoldSlingShot();
	}
	else
	{
		//移動、ジャンプアクションを行う
		mode_game->ui_.SetDrawPlayerUIFlag(true);
		camera::Camera::GetInstance()->SetStatus(camera::Camera::STATUS::MOVE);
		mode_game->ui_.shoot_ui_.SetDrawShootGuide(false);
		shoot_charge_effect_flag_ = true;

		VECTOR old_position = position_;

		Jump();
		Move();

		//プレイヤー移動によるカメラ移動
		VECTOR camera_diff = camera::Camera::GetInstance()->GetPosition();
		VECTOR player_diff = VSub(position_, old_position);

		if (VSize(player_diff) < 1.0f)
			player_diff = { 0,0,0 };

		camera_diff = VAdd(camera_diff, player_diff);
		camera::Camera::GetInstance()->SetPosition(camera_diff);
		camera::Camera::GetInstance()->SetTarget(VAdd(position_, VGet(0.0f, 60.0f, 0.0f)));

		SwitchPlayerStatus();
		DecideForwardDirection();
	}
}