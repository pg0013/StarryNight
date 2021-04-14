/**
 * @file    CameraState.h
 * @brief  カメラ状態の基底クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#pragma once
#include"appframe.h"

namespace starrynight
{
	namespace camera
	{
		class Camera;

		/**
		 * @brief カメラ状態の基底クラス
		 */
		class CameraState
			: public ::state::StateBase
		{
		public:
			virtual ~CameraState(){}

			/**
			 * @brief　 状態遷移時に一度行われる入り口処理
			 *
			 * @param  _camera インスタンスを持つクラスの参照
			 */
			virtual void Enter(Camera& _camera) {}

			/**
			 * @brief　 状態遷移時に一度行われる終了処理
			 *
			 * @param  _camera インスタンスを持つクラスの参照
			 */
			virtual void Exit(Camera& _camera) {}

			/**
			 * @brief　 状態遷移を行うか判定する入力処理
			 *
			 * @param  _camera インスタンスを持つクラスの参照
			 */
			virtual void Input(Camera& _camera) { }

			/**
			* @brief　 毎フレーム行われる更新処理
			 *
			 * @param  _camera インスタンスを持つクラスの参照
			 */
			virtual void Update(Camera& _camera) {}
		};
	}
}
