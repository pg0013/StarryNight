/**
 * @file    StateBase.h
 * @brief  ステートマシーンの基底クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/12
 */

#pragma once

namespace state
{
	class StateBase
	{
	public:
		virtual ~StateBase(){}

		/**
		 * @brief　 状態遷移時に一度行われる入り口処理
		 */
		virtual void Enter(){}

		/**
		 * @brief　 状態遷移時に行われる出口処理
		 *
		 */
		virtual void Exit(){}

		/**
		 * @brief　 毎フレーム行われる更新処理
		 */
		virtual void Update(){}
	};
}
