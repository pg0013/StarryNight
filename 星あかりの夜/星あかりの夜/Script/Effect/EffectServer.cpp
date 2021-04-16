/**
 * @file    EffectServer.h
 * @brief	エフェクト管理を行うサーバー
 *
 * @author Takuya Fujisawa
 * @date   2021/02/09
 */

#include "EffectServer.h"

using namespace starrynight::effect;

EffectServer::EffectServer()
{
	effect_list_.clear();
	add_list_.clear();
	delete_list_.clear();
}

EffectServer::~EffectServer()
{
	Clear();
}

void EffectServer::Clear()
{
	for (auto&& iter : effect_list_)
	{
		iter.reset();
	}

	effect_list_.clear();
}

void EffectServer::Add(const std::shared_ptr<EffectBase>& _Effect)
{
	add_list_.push_back(_Effect);
}

void EffectServer::Delete(const std::shared_ptr<EffectBase>& _Effect)
{
	delete_list_.push_back(_Effect);
}

void EffectServer::AddListEffect()
{
	for (auto&& iter : add_list_)
	{
		effect_list_.push_back(iter);
	}
	add_list_.clear();
}

void EffectServer::DeleteListEffect()
{
	for (auto&& iter_del : delete_list_)
	{
		for (auto iter = effect_list_.begin(); iter != effect_list_.end();)
		{
			//削除リストと一致したエフェクトを削除
			if ((*iter) == (iter_del))
			{
				iter->reset();
				//イテレータを削除し、次に進める
				iter = effect_list_.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
	delete_list_.clear();
}

void EffectServer::Process()
{
	//追加リストのオブジェクトを追加
	AddListEffect();

	//サーバーにあるオブジェクトのProcessを呼び出し
	for (auto&& iter : effect_list_)
	{
		(iter)->Process();
	}

	//削除リストのオブジェクトを削除
	DeleteListEffect();

	//再生中のエフェクトを更新
	UpdateEffekseer3D();
}

void EffectServer::Render()
{
	//サーバーにあるエフェクトのRenderを呼び出し
	for (auto&& iter : effect_list_)
	{
		(iter)->Render();
	}
	DrawEffekseer3D();
}