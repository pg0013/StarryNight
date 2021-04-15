/**
 * @file    EnemyState.h
 * @brief  敵の状態遷移クラスのインターフェース
 *
 * @author Takuya Fujisawa
 * @date   2021/04/12
 */

#pragma once
#include"appframe.h"

namespace starrynight
{
	namespace enemy
	{
		class Enemy;

		/**
		 * @brief 敵の状態基底クラス
		 */
		class EnemyState :
			public ::state::StateBase
		{
		public:
			EnemyState() {}

			virtual ~EnemyState() {}

			/**
			 * @brief 状態遷移時に一度行われる入り口処理
			 *
			 * @param  _enemy インスタンスを持つクラスの参照
			 */
			virtual void Enter(Enemy& _enemy) {}

			/**
			 * @brief 状態遷移時に一度行われる終了処理
			 *
			 * @param  _enemy インスタンスを持つクラスの参照
			 */
			virtual void Exit(Enemy& _enemy) {}

			/**
			 * @brief 状態遷移を行うか判定する入力処理
			 *
			 * @param  _enemy インスタンスを持つクラスの参照
			 */
			virtual void Input(Enemy& _enemy) { }

			/**
			* @brief 毎フレーム行われる更新処理
			 *
			 * @param  _enemy インスタンスを持つクラスの参照
			 */
			virtual void Update(Enemy& _enemy) {}
		};
	}
}
