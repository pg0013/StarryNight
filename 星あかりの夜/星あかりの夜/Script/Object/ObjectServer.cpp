/**
 * @file    ObjectServer.cpp
 * @brief	すべてのオブジェクトを管理するクラス
				vectorコンテナでオブジェクトの生成、演算、描画、消去の管理を行う
 *
 * @author Takuya Fujisawa
 * @date   2020/12/15
 */
#include "ObjectServer.h"
using namespace starrynight::object;

ObjectServer::ObjectServer()
{
	object_list_.clear();
	add_list_.clear();
	delete_list_.clear();
}

ObjectServer::~ObjectServer()
{
	Clear();
}

void ObjectServer::Clear()
{
	for (auto&& iter : object_list_)
	{
		iter.reset();
	}

	object_list_.clear();
}

void ObjectServer::Add(const std::shared_ptr<ObjectBase>& _object)
{
	add_list_.push_back(_object);
}

void ObjectServer::Delete(const std::shared_ptr<ObjectBase>& _object)
{
	delete_list_.push_back(_object);
}

void ObjectServer::AddListObject()
{
	for (auto&& iter : add_list_)
	{
		object_list_.push_back(iter);
	}
	add_list_.clear();
}

void ObjectServer::DeleteListObject()
{
	for (auto&& iter_del : delete_list_)
	{
		for (auto iter = object_list_.begin(); iter != object_list_.end();)
		{
			//削除リストと一致したオブジェクトを削除
			if ((*iter) == (iter_del))
			{
				iter->reset();
				//イテレータを削除し、次に進める
				iter = object_list_.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
	delete_list_.clear();
}

void ObjectServer::Process()
{
	//追加リストのオブジェクトを追加
	AddListObject();

	//サーバーにあるオブジェクトのProcessを呼び出し
	for (auto&& iter : object_list_)
	{
		(iter)->Process();
	}

	//削除リストのオブジェクトを削除
	DeleteListObject();
}

void ObjectServer::Render()
{
	//サーバーにあるオブジェクトのRenderを呼び出し
	for (auto&& iter : object_list_)
	{
		(iter)->Render();
	}
}