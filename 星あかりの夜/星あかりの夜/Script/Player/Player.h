/**
 * @file    Player.h
 * @brief  ゲームモード内のプレイヤークラス
 *
 * @author Takuya Fujisawa
 * @date    202012/16
 */

#pragma once
#include"appframe.h"
#include"../Object/ObjectBase.h"

namespace starrynight
{
	namespace player
	{
		class Player :
			public object::ObjectBase
		{
		public:
			Player();
			~Player();

			/**
			 * @brief オブジェクトの種類を返す.
			 *
			 * @param ObjectBase::OBJECT_TYPE::PLAYER
			 */
			virtual OBJECT_TYPE GetObjectType() override { return ObjectBase::OBJECT_TYPE::PLAYER; }

			/**
			 * @brief　初期化処理.
			 */
			virtual void Initialize();

			/**
			 * @brief　演算処理.
			 */
			virtual void Process();

			/**
			 * @brief　描画処理.
			 */
			virtual void Render() override;

			/**
			 * @brief　コントローラ入力検知処理.
			 */
			void Input();

		private:
			parameters::Parameters player_param_;
			handle model_handle_;
		};
	}
}