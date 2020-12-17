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
	for (auto iter = object_list_.begin(); iter != object_list_.end(); iter++)
	{
		delete (*iter);
	}

	object_list_.clear();
}

void ObjectServer::Add(ObjectBase* _object)
{
	add_list_.push_back(_object);
}

void ObjectServer::Delete(ObjectBase* _object)
{
	delete_list_.push_back(_object);
}

void ObjectServer::AddListObject()
{
	for (auto iter = add_list_.begin(); iter != add_list_.end(); iter++)
	{
		object_list_.push_back((*iter));
	}
	add_list_.clear();
}

void ObjectServer::DeleteListObject()
{
	for (auto iter_del = delete_list_.begin(); iter_del != delete_list_.end(); iter_del++)
	{
		for (auto iter = object_list_.begin(); iter != object_list_.end(); iter++)
		{
			//削除リストと一致したオブジェクトを削除
			if ((*iter) == (*iter_del))
			{
				delete (*iter);
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
	for (auto iter = object_list_.begin(); iter != object_list_.end(); iter++)
	{
		(*iter)->Process();
	}

	//削除リストのオブジェクトを削除
	DeleteListObject();
}

void ObjectServer::Render()
{
	//サーバーにあるオブジェクトのRenderを呼び出し
	for (auto iter = object_list_.begin(); iter != object_list_.end(); iter++)
	{
		(*iter)->Render();
	}
}