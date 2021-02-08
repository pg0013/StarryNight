/**
 * @file    UIManager.h
 * @brief  ƒQ[ƒ€“àUIŠÇ—ƒNƒ‰ƒX
 *
 * @author Takuya Fujisawa
 * @date    202102/08
 */

#include "UIManager.h"

using namespace starrynight::ui;

UIManager::UIManager()
{
	param_.LoadImagParameters("ui");
}

UIManager::~UIManager()
{
}

void UIManager::Initialize()
{
	shoot_ui_.Initialize();
}

void UIManager::Terminate()
{
	shoot_ui_.Terminate();
}

void UIManager::Process()
{
	shoot_ui_.Process();
}

void UIManager::Render()
{
	shoot_ui_.Render();
}