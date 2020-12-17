/**
 * @file    ModeGame.cpp
 * @brief  ÉQÅ[ÉÄÉÇÅ[ÉhÇÃèàóù
 *
 * @author Takuya Fujisawa
 * @date    202012/09
 */
#include "ModeGame.h"
#include"../ApplicationMain.h"
//#include<Windows.h>

using namespace starrynight;

ModeGame::ModeGame()
{
	camera_.Initialize();
	stage_.Initialize();
}

ModeGame::~ModeGame()
{

}

bool ModeGame::Initialize()
{
	return true;
}

bool ModeGame::Terminate()
{
	return true;
}

bool ModeGame::Process()
{
	camera_.Process();

	return true;
}

bool ModeGame::Render()
{
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	camera_.Render();
	stage_.Render();

#ifdef DEBUG_FUNCTION
	camera_.DrawDebugMenu();
	utility::Draw3DAxis(10.f, camera_.GetTarget());
#endif

	return true;
}