/**
 * @file    CameraMoveState.h
 * @brief  移動カメラ状態のクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/04/14
 */

#pragma once
#include "CameraState.h"

namespace starrynight
{
	namespace camera
	{
		/**
		 * @brief 移動カメラ状態のクラス
		 */
		class CameraMoveState :
			public CameraState
		{
		public:
			CameraMoveState();
			~CameraMoveState();

			/**
			 * @brief 状態遷移時に一度行われる入り口処理
			 *
			 * @param  _camera インスタンスを持つクラスの参照
			 */
			void Enter(Camera& _camera) override;

			/**
			 * @brief 状態遷移時に一度行われる終了処理
			 *
			 * @param  _camera インスタンスを持つクラスの参照
			 */
			void Exit(Camera& _camera) override;

			/**
			 * @brief 状態遷移を行うか判定する入力処理
			 *
			 * @param  _camera インスタンスを持つクラスの参照
			 */
			void Input(Camera& _camera) override;

			/**
			* @brief 毎フレーム行われる更新処理
			 *
			 * @param  _camera インスタンスを持つクラスの参照
			 */
			void Update(Camera& _camera) override;

		private:
			/**
			 * @brief 移動カメラの処理
			 *
			 * @param  _camera　インスタンスを持つクラスの参照
			 */
			void MoveCamera(Camera& _camera);
		};
	}
}
