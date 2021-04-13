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
#include"EnemyState.h"
#include<unordered_map>

namespace starrynight
{
	namespace enemy
	{
		/**
		 * @brief  敵オブジェクトクラス
		 */
		class Enemy :
			public object::ObjectBase
		{
		public:
			Enemy(std::string _handle_name);
			~Enemy();

			/**
			 * @brief オブジェクトの種類を返す.
			 *
			 * @param ObjectBase::OBJECT_TYPE::PLAYER
			 */
			OBJECT_TYPE GetObjectType() const override { return ObjectBase::OBJECT_TYPE::ENEMY; }

			/**
			 * @brief　初期化処理.
			 */
			void Initialize() override;

			/**
			 * @brief　演算処理.
			 */
			void Process() override;

			/**
			 * @brief　描画処理.
			 */
			void Render() override;

			/**
			 * @brief 状態遷移用定数
			 */
			enum class ANIM_STATUS
			{
				NONE,
				WAIT,
				MOVE,
				RUN,
				RUSH,
				ATTACK
			};

			/**
			 * @brief 移動状態用定数
			 */
			enum class MOVE_STATUS
			{
				WAIT,
				STRAIGHT,
				TRACKING,
				ATTACK,
				ESCAPE
			};

			/**
			 * @brief　 プレイヤー検出距離を取得する
			 *
			 * @return   プレイヤー検出距離
			 */
			float GetDetectLength() const { return detect_length_; }

			/**
			 * @brief　 敵の回転速度を取得する
			 *
			 * @return   敵の回転速度
			 */
			float GetRotationSpeed() const { return rot_speed_; }

			/**
			 * @brief　 敵の歩く速度を取得する
			 *
			 * @return   歩く速度
			 */
			float GetWalkSpeed() const { return walk_speed_; }

			/**
			 * @brief　 敵の走る速度を取得する
			 *
			 * @return   走る速度
			 */
			float GetRunSpeed() const { return run_speed_; }

			/**
			 * @brief　 アニメーション再生時間を取得する
			 *
			 * @return   アニメーション再生時間
			 */
			float GetAnimPlayTime() const { return anim_play_time_; }

			/**
			 * @brief　 アニメーション総再生時間を取得する
			 *
			 * @return   アニメーション総再生時間
			 */
			float GetAnimTotalTime() const { return anim_total_time_; }

			/**
			 * @brief　 移動状態を取得する
			 *
			 * @return   敵の移動状態
			 */
			MOVE_STATUS GetMoveStatus() const { return move_status_; }

			/**
			 * @brief　 敵の移動状態を設定する
			 *
			 * @param  _status　移動状態
			 */
			void SetMoveStatus(const MOVE_STATUS& _status) { move_status_ = _status; }

			/**
			 * @brief　 敵のアニメーション状態を設定する
			 *
			 * @param  _status　アニメーション状態
			 */
			void SetAnimStatus(const ANIM_STATUS& _status) { anim_status_ = _status; }

			/**
			 * @brief　 移動処理
			 *
			 */
			void Move(const VECTOR& _move);

			/**
			 * @brief　 指定した状態に遷移する
			 *
			 * @param  _state_name　状態の名前
			 */
			void ChangeEnemyState(const std::string& _state_name);

			sound::PlayWAVE se_;//敵モーションSE
			sound::PlayWAVE attention_se_;//敵プレイヤー発見SE

		private:
			/**
			 * @brief　 エネミーのさまよう移動処理
			 *
			 * @return　移動方向
			 */
			VECTOR DecideMoveAmount();

			/**
			 * @brief　 プレイヤーを追跡する処理
			 *
			 * @param  _move　移動方向ベクトル
			 * @return   移動方向
			 */
			VECTOR Tracking(VECTOR& _move);

			/**
			 * @brief　 プレイヤーを攻撃する処理
			 *
			 * @param  _move　移動方向ベクトル
			 * @return   移動方向
			 */
			VECTOR Attack(VECTOR& _move);

			/**
			 * @brief　 状態遷移を決定する処理
			 *
			 * @param  _old_status	前フレームの状態
			 */
			void SwitchStatus();

			/**
			 * @brief　 アニメーション切り替え関数
			 *
			 * @param  _old_status	遷移元の状態
			 */
			void SwitchEnemyAnimation(const ANIM_STATUS& _old_status);

			/**
			 * @brief SEの切り替えを行う処理.
			 */
			void SwitchEnemySound();

		private:
			//エネミーパラメータ保持クラス
			EnemyParameters enemy_param_;

			//状態遷移保持ポインタ
			EnemyState* enemy_state_;

			//状態遷移管理マップ
			std::unordered_map<std::string, EnemyState*> state_map_;

			//現在の状態を保持する変数
			ANIM_STATUS anim_status_;
			MOVE_STATUS move_status_;

			//jsonから読み込む外部パラメーター
			float walk_speed_;//移動速度
			float run_speed_;//移動速度
			float rot_speed_;//回転速度
			float detect_length_;//プレイヤー検出範囲

			bool attacked_flag_;//攻撃したことを記録するフラグ

			float random_rot_direction_;//回転方向をランダムで変化させる変数

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
