/**
 * @file    EnemyAttackState.h
 * @brief  敵の攻撃処理クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/12
 */

#pragma once
#include "EnemyState.h"

namespace starrynight
{
	namespace enemy
	{
		/**
		 * @brief 敵の攻撃状態クラス
		 */
		class EnemyAttackState :
			public EnemyState
		{
		public:
			EnemyAttackState();

			~EnemyAttackState();

			/**
			 * @brief　 状態遷移時に一度行われる入り口処理
			 *
			 * @param  _enemy インスタンスを持つクラスの参照
			 */
			void Enter(Enemy& _enemy) override;

			/**
			 * @brief　 状態遷移時に一度行われる終了処理
			 *
			 * @param  _enemy インスタンスを持つクラスの参照
			 */
			void Exit(Enemy& _enemy) override;

			/**
			 * @brief　 状態遷移を行うか判定する入力処理
			 */
			void Input(Enemy& _enemy) override;

			/**
			* @brief　 毎フレーム行われる更新処理
			 *
			 * @param  _enemy インスタンスを持つクラスの参照
			 */
			void Update(Enemy& _enemy) override;
		};
	}
}
