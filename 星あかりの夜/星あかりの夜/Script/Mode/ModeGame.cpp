/**
 * @file    ModeGame.cpp
 * @brief  ゲームモードの処理
 *
 * @author Takuya Fujisawa
 * @date    202012/09
 */

#include "ModeGame.h"
#include"ModeGameClear.h"
#include"ModeGameOver.h"
#include"ModeLoading.h"
#include"ModeOverlay.h"
#include"ModePauseMenu.h"
#include"../Player/Player.h"

using namespace starrynight::mode;

ModeGame::ModeGame(std::string _stage_name)
{
	stage_name_ = _stage_name;
	stop_object_process_ = false;
	player_star_num_ = 0;
	game_score_ = 0;
	fade_count_ = 0;
	nextmode_count_ = 0;
	pushed_flag_ = false;
	pause_flag_ = false;
	score_rank_ = SCORE_RANK::LOW;
	result_ = -1;
	gameover_flag_ = false;

	//ステージ規定スコアを設定
	if (std::equal(stage_name_.begin(), stage_name_.end(), "haru_A"))
	{
		SetStageRegulationsScore(30000);
	}
	else if (std::equal(stage_name_.begin(), stage_name_.end(), "haru_B"))
	{
		SetStageRegulationsScore(50000);
	}
	else
	{
		SetStageRegulationsScore(70000);
	}

	stage_shadowmap_ = MakeShadowMap(8192, 8192);
	object_shadowmap_ = MakeShadowMap(8192, 8192);
}

ModeGame::~ModeGame()
{
}

bool ModeGame::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	//プレイヤーを生成
	std::shared_ptr<player::Player> player = std::make_shared<player::Player>(stage_name_);
	object_server_.Add(player);

	//カメラ、ステージ、UIの初期化
	camera_.Initialize();
	stage_.Initialize(stage_name_);
	ui_.Initialize();

	//ライトとシャドウマップの設定
	SetShadowMapDrawArea(stage_shadowmap_, VGet(-2500.0f, -1.0f, -2500.0f), VGet(2500.0f, 2500.0f, 2500.0f));
	SetShadowMapDrawArea(object_shadowmap_, VGet(-2500.0f, -1.0f, -2500.0f), VGet(2500.0f, 2500.0f, 2500.0f));
	VECTOR light_direction = VGet(0.1f, -0.05f, -0.1f);
	if (std::equal(stage_name_.begin(), stage_name_.end(), "haru_C"))
	{
		light_direction = VGet(-0.1f, -0.05f, 0.1f);
	}

	SetLightDirection(light_direction);
	SetShadowMapLightDirection(stage_shadowmap_, light_direction);
	SetShadowMapLightDirection(object_shadowmap_, light_direction);
	SetLightAmbColor(GetColorF(-0.05f, -0.05f, -0.05f, 0.0f));

	appframe::ApplicationBase::GetInstance()->bgm_.Load("Resource/sound/ingame_bgm.wav");
	appframe::ApplicationBase::GetInstance()->bgm_.SetVolume(1.0f);
	appframe::ApplicationBase::GetInstance()->bgm_.SetLoopCount(XAUDIO2_LOOP_INFINITE);
	appframe::ApplicationBase::GetInstance()->bgm_.PlayWithLoop(0.0f, 234.5f);

	std::shared_ptr<mode::ModeLoading> mode_loading = std::dynamic_pointer_cast<mode::ModeLoading>( ::mode::ModeServer::GetInstance()->Get("Loading"));
	mode_loading->SetNextMode();

	return true;
}

bool ModeGame::Terminate()
{
	::mode::ModeBase::Terminate();
	object_server_.Clear();
	effect_server_.Clear();
	stage_.ClearHandle();
	ui_.Terminate();

	DeleteShadowMap(stage_shadowmap_);
	DeleteShadowMap(object_shadowmap_);

	appframe::ApplicationBase::GetInstance()->bgm_.Pause();
	appframe::ApplicationBase::GetInstance()->bgm_.Destroy();

	return true;
}

bool ModeGame::Process()
{
	::mode::ModeBase::Process();

	if (!stop_object_process_)
	{
		object_server_.Process();
		effect_server_.Process();
	}

	camera_.Process();
	ui_.Process();

	CheckIsGameOver();

	Input();
	NextMode();

	return true;
}

bool ModeGame::Render()
{
	::mode::ModeBase::Render();

	//Zバッファの設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	camera_.Render();

	//動かないステージを一度だけシャドウマップに描画
	if (GetModeCount() == 1)
	{
		ShadowMap_DrawSetup(stage_shadowmap_);
		stage_.Render();
		ShadowMap_DrawEnd();
	}

	//動くオブジェクトをシャドウマップに描画
	ShadowMap_DrawSetup(object_shadowmap_);
	object_server_.Render();
	ShadowMap_DrawEnd();

	//ステージとオブジェクトを描画
	SetUseShadowMap(0, stage_shadowmap_);
	SetUseShadowMap(1, object_shadowmap_);
	stage_.Render();
	object_server_.Render();
	SetUseShadowMap(1, -1);

	effect_server_.Render();
	ui_.Render();

#ifdef DEBUG_FUNCTION
	int x, y;
	camera_.DrawDebugMenu();
	utility::DrawModelDebugInfo(resource::ResourceServer::GetModelHandle("player"), "Player", 7);
	utility::Draw3DAxis(10.f, camera_.GetTarget());
	x = 0; y = 11 * DEBUG_FONT_SIZE;
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- star    : %d", y / DEBUG_FONT_SIZE, GetPlayerStarNum()); y += DEBUG_FONT_SIZE;
	y = 13 * DEBUG_FONT_SIZE;
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- RegScore    : %d", y / DEBUG_FONT_SIZE, GetStageRegulationsScore()); y += DEBUG_FONT_SIZE;
	DrawFormatString(x, y, DEBUG_COLOR, "%2d   -- GameScore   : %d", y / DEBUG_FONT_SIZE, GetGameScore()); y += DEBUG_FONT_SIZE;
#endif

	return true;
}

void ModeGame::SetNextMode(const int& _count, const int& _fade_count, const int& _result)
{
	pushed_flag_ = true;

	nextmode_count_ = _count;
	fade_count_ = _fade_count;
	result_ = _result;
}

void ModeGame::Input()
{
	if (pushed_flag_ == true)
		return;

	int trigger_key = appframe::ApplicationBase::GetInstance()->GetTriggerKey();

	//STARTボタンでポーズメニューを表示
	if (trigger_key & PAD_INPUT_8)
	{
		pushed_flag_ = true;
		pause_flag_ = true;

		nextmode_count_ = 1;

		appframe::ApplicationBase::GetInstance()->se_.Load("Resource/sound/se2.wav");
		appframe::ApplicationBase::GetInstance()->se_.Play();

		//ダッキング
		appframe::ApplicationBase::GetInstance()->bgm_.Fade(XAudio2DecibelsToAmplitudeRatio(-12.0f), 1.0f);
	}
}

void ModeGame::CheckIsClearStage()
{
	if (IsClearStage() == true)
	{
		//星座によって完成エフェクトの時間が違うので、次のモードへ移行する時間をステージごとに設定する
		if (GetStageName() == "haru_A")
		{
			SetNextMode(420, 40, GAME_CLEAR);
		}
		else if (GetStageName() == "haru_B")
		{
			SetNextMode(480, 40, GAME_CLEAR);
		}
		else if (GetStageName() == "haru_C")
		{
			SetNextMode(450, 40, GAME_CLEAR);
		}
	}
	else
	{
		//星を発射しつくしてゲームクリア判定がなければゲームオーバーへ移行
		if (GetStageStarNum() == 0)
		{
			gameover_flag_ = true;

			std::shared_ptr<mode::ModeGame> mode_game = mode::ModeGame::GetModeGame();
			mode_game->SetNextMode(300, 60, GAME_OVER);
		}
	}
}

void ModeGame::CheckIsGameOver()
{
	if (gameover_flag_)
		return;

	for (auto&& iter : (*object_server_.List()))
	{
		if (iter->GetObjectType() == object::ObjectBase::OBJECT_TYPE::PLAYER)
		{
			std::shared_ptr<player::Player> player = std::dynamic_pointer_cast<player::Player>(iter);
			//HPがあればゲームオーバーではない
			if (player->GetPlayerHP() > 0)
				return;
		}
	}

	gameover_flag_ = true;

	//ゲームオーバーへ移行
	SetNextMode(300, 60, GAME_OVER);
}

void ModeGame::NextMode()
{
	if (pushed_flag_ == false)
		return;

	nextmode_count_--;

	//カウンタがフェード開始時間になったら、オーバーレイを生成しBGMをフェード
	if (nextmode_count_ == fade_count_ &&
		pause_flag_ == false)
	{
		std::shared_ptr<ModeOverlay> mode_overlay = std::make_shared<ModeOverlay>();
		mode_overlay->Fade(fade_count_, FADE_OUT);
		::mode::ModeServer::GetInstance()->Add(mode_overlay, 0, "Overlay");

		appframe::ApplicationBase::GetInstance()->bgm_.Fade(0.0f, static_cast<float>(fade_count_) / 60.0f);
	}

	if (nextmode_count_ == 0)
	{
		if (pause_flag_)
		{
			//ポーズメニューへ移行
			std::shared_ptr<ModePauseMenu> mode_pausemenu = std::make_shared<ModePauseMenu>();
			::mode::ModeServer::GetInstance()->Add(mode_pausemenu, 1, "PauseMenu");
			ui_.SetDrawUIFlag(false);
			//モードを削除しないので、フラグを初期化
			pause_flag_ = false;
			pushed_flag_ = false;
			return;
		}

		if (result_ == GAME_CLEAR)
		{
			//ゲームクリアへ移行し、モードを削除
			std::shared_ptr<ModeGameClear> mode_gameclear = std::make_shared<ModeGameClear>(game_score_, stage_name_);
			::mode::ModeServer::GetInstance()->Add(mode_gameclear, 0, "GameClear");
			::mode::ModeServer::GetInstance()->Del(shared_from_this());
		}
		else if (result_ == GAME_OVER)
		{
			//ゲームオーバー並行し、モードを削除
			std::shared_ptr<ModeGameOver> mode_gameover = std::make_shared<ModeGameOver>(stage_name_);
			::mode::ModeServer::GetInstance()->Add(mode_gameover, 0, "GameOver");
			::mode::ModeServer::GetInstance()->Del(shared_from_this());
		}
	}
}