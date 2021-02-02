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
#include"PlayerParameters.h"

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
			/**
			 * @brief　 移動処理
			 *
			 */
			void Move();

			/**
			 * @brief　 ジャンプ処理
			 *
			 */
			void Jump();

			/**
			 * @brief	パチンコを構える処理.
			 *
			 */
			void HoldSlingShot();

			/**
			 * @brief 状態遷移用定数
			 */
			enum class STATUS
			{
				NONE,
				WAIT,
				WALK,
				RUN,
				JUMP_START,
				JUMP_LOOP,
				JUMP_END,
				SHOOT,
				_EOT_
			};

			/**
			 * @brief　 アニメーション切り替え関数
			 *
			 * @param  _old_status	遷移元の状態
			 */
			void SwitchAnimation(STATUS _old_status);

		private:
			//プレイヤーパラメータ保持クラス
			PlayerParameters player_param_;

			float move_speed_;//移動速度
			float rot_speed_;//回転速度

			float jump_speed_;//ジャンプ速度
			float gravity_;//重力
			bool jump_flag_;

			//モデルアニメーション用変数
			int anim_attach_index_;//アニメーションアタッチ番号
			int old_anim_attach_index_;//遷移元のアニメーションアタッチ番号
			float anim_rate_;//アニメーションブレンド率
			float anim_total_time_;//アニメーション総再生時間
			float anim_play_time_;//アニメーション再生時間
			bool anim_loop_flag_;//アニメーションをループするかどうか

			STATUS status_;//プレイヤーの状態保持変数
		};
	}
}