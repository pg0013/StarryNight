/**
 * @file    EffectServer.h
 * @brief	エフェクト管理を行うサーバー
 *
 * @author Takuya Fujisawa
 * @date   2021/02/09
 */

#pragma once
#include"appframe.h"
#include"EffectBase.h"
#include<vector>

namespace starrynight
{
	namespace effect
	{
		class EffectServer
		{
		public:
			EffectServer();
			~EffectServer();

			/**
			 * @brief	コンテナの中身をすべて削除
			 */
			void Clear();

			/**
			 * @brief	コンテナに追加（最前面に表示）
			 *
			 * @param [in,out]	_object	追加するオブジェクトのポインタ
			 */
			void Add(EffectBase* _effect);

			/**
			 * @brief	コンテナから削除
			 *
			 * @param [in,out]	削除するオブジェクトのポインタ
			 */
			void Delete(EffectBase* _effect);

			/**
			 * @brief	毎フレーム呼ばれる演算処理
			 */
			void Process();

			/**
			 * @brief	描画処理
			 */
			void Render();

			/**
			 * @brief	EffectBaseを格納してるコンテナへのゲッター
			 *
			 * @returns	コンテナ
			 */
			std::vector<EffectBase*>* List() { return &effect_list_; }

		private:
			/**
			 * @brief	追加リストのオブジェクトをeffect_list_に追加
			 */
			void AddListEffect();

			/**
			 * @brief	削除リストのオブジェクトを削除
			 */
			void DeleteListEffect();

			std::vector<EffectBase*> effect_list_; //!< オブジェクト格納コンテナ
			std::vector<EffectBase*> add_list_;	//!< 追加用コンテナ
			std::vector<EffectBase*> delete_list_; //!< 削除用コンテナ
		};
	}
}



