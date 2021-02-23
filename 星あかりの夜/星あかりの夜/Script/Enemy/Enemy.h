/**
 * @file    Enemy.h
 * @brief  敵オブジェクトクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/20
 */

#pragma once
#include"appframe.h"
#include "../Object/ObjectBase.h"
#include"EnemyParameters.h"

namespace starrynight
{
	namespace enemy
	{
		class Enemy :
			public object::ObjectBase
		{
		public:
			Enemy();
			~Enemy();

			/**
			 * @brief オブジェクトの種類を返す.
			 *
			 * @param ObjectBase::OBJECT_TYPE::PLAYER
			 */
			virtual OBJECT_TYPE GetObjectType() override { return ObjectBase::OBJECT_TYPE::ENEMY; }

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
			 * @brief　 エネミーのさまよう移動処理
			 *
			 * @return　移動方向
			 */
			VECTOR Wander();

			/**
			 * @brief　 移動処理
			 *
			 */
			void Move(VECTOR _move);

			/**
			 * @brief 状態遷移用定数
			 */
			enum class STATUS
			{
				NONE,
				WAIT,
				MOVE,
				RUN,
				RUSH,
				ATTACK
			};

			enum class MOVE_STATUS
			{
				WAIT,
				STRAIGHT,
				TRACKING,
				ATTACK,
				ESCAPE
			};

			/**
			 * @brief　 アニメーション切り替え関数
			 *
			 * @param  _old_status	遷移元の状態
			 */
			void SwitchAnimation(STATUS _old_status);


		private:
			//エネミーパラメータ保持クラス
			EnemyParameters enemy_param_;

			STATUS status_;
			MOVE_STATUS move_status_;

			//jsonから読み込む外部パラメーター
			float walk_speed_;//移動速度
			float run_speed_;//移動速度
			float rot_speed_;
			float detect_length_;

			int start_frame_;

			bool attacked_flag_;

			float random_rot_direction_;

			//モデルアニメーション用変数
			int anim_attach_index_;//アニメーションアタッチ番号
			int old_anim_attach_index_;//遷移元のアニメーションアタッチ番号
			float anim_rate_;//アニメーションブレンド率
			float anim_total_time_;//アニメーション総再生時間
			float anim_play_time_;//アニメーション再生時間
			bool anim_loop_flag_;//アニメーションをループするかどうか
		};
	}
}


