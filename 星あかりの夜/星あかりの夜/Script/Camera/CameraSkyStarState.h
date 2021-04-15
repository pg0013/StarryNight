/**
 * @file    CameraSkyStarState.h
 * @brief  星座完成時のカメラ状態クラス
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
		 * @brief  星座完成時のカメラ状態クラス
		 */
		class CameraSkyStarState :
			public CameraState
		{
		public:
			CameraSkyStarState();
			~CameraSkyStarState();

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
		};
	}
}
