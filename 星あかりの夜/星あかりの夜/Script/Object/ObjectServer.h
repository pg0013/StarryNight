/**
 * @file    ObjectServer.h
 * @brief  オブジェクトの管理を行うサーバー
 *
 * @author Takuya Fujisawa
 * @date   2020/12/15
 */

#pragma once
#include"appframe.h"
#include"ObjectBase.h"
#include<vector>

namespace starrynight
{
	namespace object
	{
		/**
		* @class	ObjectServer
		*
		* @brief すべてのオブジェクトを管理するクラス
		* @details vectorコンテナでオブジェクトの生成、演算、描画、消去の管理を行う
		*
		*/
		class ObjectServer
		{
		public:
			ObjectServer();
			~ObjectServer();

			/**
			 * @fn	void ObjectServer::Clear();
			 *
			 * @brief	コンテナの中身をすべて削除
			 */
			void Clear();

			/**
			 * @fn	void ObjectServer::Add(ObjectBase* _object);
			 *
			 * @brief	コンテナに追加（最前面に表示）
			 *
			 * @param [in,out]	_object	追加するオブジェクトのポインタ
			 */
			void Add(ObjectBase* _object);

			/**
			 * @fn	void ObjectServer::Delete(ObjectBase* _object);
			 *
			 * @brief	コンテナから削除
			 *
			 * @param [in,out]	削除するオブジェクトのポインタ
			 */
			void Delete(ObjectBase* _object);

			/**
			 * @fn	void ObjectServer::Process(Game& _game);
			 *
			 * @brief	毎フレーム呼ばれる演算処理
			 */
			void Process();

			/**
			 * @fn	void ObjectServer::Draw(Game& _game);
			 *
			 * @brief	描画処理
			 */
			void Render();

			/**
			 * @fn	std::vector<ObjectBase*> ObjectServer::*List()
			 *
			 * @brief	ObjectBaseを格納してるコンテナへのゲッター
			 *
			 * @returns	コンテナ
			 */
			std::vector<ObjectBase*>* List() { return &object_list_; }

		private:
			/**
			 * @fn	void ObjectServer::AddListObject();
			 *
			 * @brief	追加リストのオブジェクトをobject_list_に追加
			 */
			void AddListObject();

			/**
			 * @fn	void ObjectServer::DeleteListObject();
			 *
			 * @brief	削除リストのオブジェクトを削除
			 */
			void DeleteListObject();

			std::vector<ObjectBase*> object_list_; //!< オブジェクト格納コンテナ
			std::vector<ObjectBase*> add_list_;	//!< 追加用コンテナ
			std::vector<ObjectBase*> delete_list_; //!< 削除用コンテナ
		};
	}
}
