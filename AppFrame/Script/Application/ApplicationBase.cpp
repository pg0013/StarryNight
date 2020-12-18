/**
 * @file    ApplicationBase.cpp
 * @brief  アプリケーションの基底クラスの処理
 *
 * @author Takuya Fujisawa
 * @date    20201021
 */
#include "ApplicationBase.h"
#include"../Utility/Utility.h"
#include"../Utility/DefineMacro.h"
#include"../Resource/ResourceServer.h"
#include"../Sound/PlayWAVE.h"

namespace appframe
{
	ApplicationBase* ApplicationBase::appbase_instance_ = NULL;

	ApplicationBase::ApplicationBase()
	{
		//静的メンバに自分のクラス情報を入れる
		appbase_instance_ = this;

		//変数の初期化
		key_ = 0;
		trigger_key_ = 0;
		mode_server_ = nullptr;
		fps_ = 0;
		now_time_ = 0;
		fps_check_time_ = 0;
		fps_count_ = 0;
		x_input_ = { 0 };
		d_input_ = { 0 };
	}

	ApplicationBase::~ApplicationBase() {}

	bool ApplicationBase::Initialize(HINSTANCE hinstance)
	{
		//DXライブラリのアーカイブファイル読み込みを使用する
		SetUseDXArchiveFlag(TRUE);

#ifdef DEBUG_FUNCTION
		SetLogDrawArea(1280, 0, 1920, 1080);//printfdxの描画位置を設定
		utility::CheckMemoryLeak();
		AppWindowed(TRUE);
		OutputDebugLog(TRUE);
		SetBackgroundColor(0, 255, 255, 255);
#else
		AppWindowed(FALSE);
		OutputDebugLog(FALSE);
#endif

		SetMainWindowText("星あかりの夜");
		SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);

		if (DxLib_Init() == -1)
		{
			return false;
		}
		SetDrawScreen(DX_SCREEN_BACK);

		srand((unsigned int)time(NULL));

		sound::PlayWAVE::Init();
		resource::ResourceServer::Init();

		mode_server_ = NEW mode::ModeServer();

		fps_check_time_ = GetNowHiPerformanceCount();

		return true;
	}

	bool ApplicationBase::Terminate()
	{
		delete mode_server_;

		resource::ResourceServer::Release();
		sound::PlayWAVE::Release();

		DxLib_End();

		return true;
	}

	bool ApplicationBase::Input()
	{
		int keyold = trigger_key_;

		key_ = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		GetJoypadXInputState(DX_INPUT_PAD1, &x_input_);
		GetJoypadDirectInputState(DX_INPUT_PAD1, &d_input_);

		trigger_key_ = (key_ ^ keyold) & trigger_key_;

		return true;
	}

	bool ApplicationBase::Process()
	{
		mode_server_->ProcessInit();
		mode_server_->Process();
		mode_server_->ProcessFinish();

		CalculateFPS();
		return true;
	}

	bool ApplicationBase::Render()
	{
		mode_server_->RenderInit();
		mode_server_->Render();

#ifdef DEBUG_FUNCTION
		//FPS表示
		DrawFormatString(0, 0, DEBUG_COLOR, "%2d fps : %d", 0, fps_);
		utility::Draw3DAxis(1000.f, VGet(0, 0, 0));
#endif

		mode_server_->RenderFinish();

		return true;
	}

	void ApplicationBase::AppWindowed(bool _window_mode)
	{
		ChangeWindowMode(_window_mode);
	}

	void ApplicationBase::OutputDebugLog(bool _window_mode)
	{
		SetOutApplicationLogValidFlag(_window_mode);
	}

	void ApplicationBase::CalculateFPS()
	{
		//FPS計算
		now_time_ = GetNowHiPerformanceCount();
		fps_count_++;
		if (now_time_ - fps_check_time_ > 1000000)
		{
			fps_ = fps_count_;
			fps_count_ = 0;
			fps_check_time_ = now_time_;
		}
	}
}