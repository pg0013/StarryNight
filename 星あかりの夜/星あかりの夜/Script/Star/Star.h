/**
 * @file    Star.h
 * @brief  ステージ内のスター挙動実装クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/01/23
 */

#pragma once
#include "../Object/ObjectBase.h"

namespace starrynight
{
	namespace star
	{
		class Star :
			public object::ObjectBase
		{
		public:
			Star();
			~Star();

			/**
			 * @brief オブジェクトの種類を返す.
			 *
			 * @param ObjectBase::OBJECT_TYPE::PLAYER
			 */
			virtual OBJECT_TYPE GetObjectType() override { return ObjectBase::OBJECT_TYPE::STAR; }

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

		private:
			/**
			 * @brief 待機処理.
			 *
			 */
			void Wait();

			void Vibration();

			/**
			 * @brief　プレイヤーの後ろをついてくる処理.
			 *
			 */
			void Follow();

			/**
			 * @brief　プレイヤーの元から散らばる処理.
			 *
			 */
			void Diffusion();

			/**
			 * @brief 状態遷移用定数
			 */
			enum class STATUS
			{
				NONE,
				WAIT,
				FOLLOW,
				DIFFUSION,
				_EOT_
			};

			STATUS status_;

			float jump_height_;//ジャンプする高さ
			float jump_speed_;//
			float ground_position_y_;
			float gravity_;
			float rot_speed_;//待機時回転速度
		};
	}
}