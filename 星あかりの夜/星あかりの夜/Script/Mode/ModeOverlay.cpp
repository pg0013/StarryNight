/**
 * @file    ModeOverlay.h
 * @brief	画面遷移モード
 * 				キャプチャかフェードかを指定して画面遷移を行う
 * 				キャプチャでは、表示画面の上からオーバーレイ表示
 * 				フェードでは、画面を暗転させる
 *
 * @author Takuya Fujisawa
 * @date   2021/03/03
 */

#include"ModeOverlay.h"

using namespace starrynight::mode;

ModeOverlay::ModeOverlay()
{
	background_graph_ = 0;
	fade_count_ = 0;
	all_fade_count_ = 0;
	fade_type_ = 0;
}

ModeOverlay::~ModeOverlay()
{
}

bool ModeOverlay::Initialize()
{
	if (!::mode::ModeBase::Initialize()) { return false; }

	return true;
}

bool ModeOverlay::Terminate()
{
	::mode::ModeBase::Terminate();

	DeleteGraph(background_graph_);

	return true;
}

bool ModeOverlay::Process()
{
	::mode::ModeBase::Process();

	fade_count_--;

	if (fade_count_ <= 0)
		::mode::ModeServer::GetInstance()->Del(this);

	return true;
}

bool ModeOverlay::Render()
{
	::mode::ModeBase::Render();

	// α値を計算
	int alpha = 0;

	if (fade_type_ == FADE_IN)
		alpha = 255 * fade_count_ / all_fade_count_;
	else
		alpha = 255 - 255 * fade_count_ / all_fade_count_;

	// 上に画面を被せる
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);		// 半透明描画指定
	DrawGraph(0, 0, background_graph_, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明描画指定

	return true;
}

void ModeOverlay::Capture(int _fade_count)
{
	fade_count_ = _fade_count;
	all_fade_count_ = _fade_count;

	//画面サイズと同じスクリーンの作成
	background_graph_ = MakeGraph(appframe::SCREEN_WIDTH, appframe::SCREEN_HEIGHT);

	//画面に表示されている画像の取り込み
	GetDrawScreenGraph(0, 0, appframe::SCREEN_WIDTH, appframe::SCREEN_HEIGHT, background_graph_);
}

void ModeOverlay::Fade(int _fade_count, int _fade_type)
{
	fade_count_ = _fade_count;
	all_fade_count_ = _fade_count;
	fade_type_ = _fade_type;

	//画面サイズと同じスクリーンの作成
	background_graph_ = MakeGraph(appframe::SCREEN_WIDTH, appframe::SCREEN_HEIGHT);

	//黒画像の不透明度を調節して暗転を行う方法はうまくいかなかったので、
	//画面外から描画されていない部分をスクリーンとして切り取り、暗転用画像として使用している。
	GetDrawScreenGraph(2 * appframe::SCREEN_WIDTH, 2 * appframe::SCREEN_HEIGHT, appframe::SCREEN_WIDTH, appframe::SCREEN_HEIGHT, background_graph_);
}