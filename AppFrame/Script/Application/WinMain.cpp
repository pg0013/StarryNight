/**
 * @file    WinMain.cpp
 * @brief  アプリケーションのエントリーポイント
 *
 * @author Takuya Fujisawa
 * @date    20201021
 */

#include"../appframe.h"

int WINAPI WinMain(HINSTANCE _hinstance, HINSTANCE _hprev_instance, LPSTR _lpcmdline, int _ncmdshow)
{
	appframe::ApplicationBase* appbase = appframe::ApplicationBase::GetInstance();

	if (!appbase) { return 0; }

	if (appbase->Initialize(_hinstance) == false) { return 0; }

	//メインループ
	while (ProcessMessage() == 0)
	{
		appbase->Input();
		appbase->Process();
		appbase->Render();
	}

	appbase->Terminate();

	return 0;
}