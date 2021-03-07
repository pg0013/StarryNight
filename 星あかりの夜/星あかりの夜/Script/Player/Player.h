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
			 * @brief　 ダメージを受けたことを記録するフラグを設定する
			 *
			 * @param  _flag　ダメージを受けたフラグ
			 */
			void SetDamageFlag(bool _flag) { damage_flag_ = _flag; }

			/**
			 * @brief　 ダメージを受けたことを記録するフラグを返す
			 *
			 * @return   trueでダメージを受けた
			 */
			bool GetDamageFlag() { return damage_flag_; }

			/**
			 * @brief　 ダメージモーションを開始するフラグを設定する
			 *
			 * @param  _flag	ダメージモーションを開始するフラグ
			 */
			void SetDamageAnimFlag(bool _flag) { damage_anim_flag_ = _flag; }

			/**
			 * @brief	射撃状態かどうかを判定するフラグを取得する.
			 *
			 * @return		trueで射撃状態中
			 */
			bool GetPlayerSlingShotStatus() { return slingshot_flag_; }

			/**
			 * @brief　 射撃構え時に星が選択されたかどうか取得するフラグ
			 *
			 * @return   trueで星選択済み
			 */
			bool GetSelectedStarFlag() { return selected_skystar_flag_; }

			/**
			 * @brief　 プレイヤーの残りHPを返す
			 *
			 * @return   HP
			 */
			int GetPlayerHP() { return player_hp_; }

			/**
			 * @brief　 プレイヤーのHPを加算する
			 *
			 * @param  _addhp　加えるHP数
			 */
			void AddPlayerHP(int _addhp) { player_hp_ += _addhp; }

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
				SHOOT_START,
				SHOOT_END,
				DAMAGE,
				_EOT_
			};

			/**
			 * @brief　 プレイヤーの状態を取得
			 *
			 * @return   プレイヤー状態
			 */
			STATUS GetPlayerStatus() { return status_; }

		private:
			/**
			 * @brief　 移動処理
			 */
			void Move();

			/**
			 * @brief　 ジャンプ処理
			 */
			void Jump();

			/**
			 * @brief	パチンコを構えて星を撃つ処理.
			 */
			void HoldSlingShot();

			/**
			 * @brief パチンコを構える処理.
			 */
			void SlingShotStance();

			/**
			 * @brief 星を発射する処理.
			 */
			void Launch_Star(VECTOR _star_position);

			/**
			 * @brief 射撃構え時のプレイヤーの向きを設定する.
			 */
			void SetShootRotation();

			/**
			 * @brief. 星座との当たり判定を取得する
			 *
			 * @return 星座との当たり判定構造体
			 */
			MV1_COLL_RESULT_POLY CheckHitStar();

			/**
			 * @brief　 右スティック入力からプレイヤーの方向を決める
			 */
			void DecideForwardDirection();

			/**
			 * @brief	ダメージを受けた時の処理
			 */
			void Damage();

			/**
			 * @brief　 ステージの外に飛び出たときの処理
			 *
			 */
			void OutOfStage();

			/**
			 * @brief　 ゲームオーバー処理
			 */
			void GameOver();

			/**
			 * @brief　 プレイヤーのアクション切り替えを行う処理
			 *
			 */
			void SwitchPlayerAction();

			/**
			 * @brief　 プレイヤーの状態切り替えを行う処理
			 *
			 */
			void SwitchPlayerStatus();

			/**
			 * @brief　 アニメーション切り替え関数
			 *
			 * @param  _old_status	遷移元の状態
			 */
			void SwitchPlayerAnimation(STATUS _old_status);

			/**
			 * @brief　 別のアニメーションに移行するときにアニメーションをブレンドする処理
			 */
			void PlayerAnimationBlend();

		private:
			//プレイヤーパラメータ保持クラス
			PlayerParameters player_param_;

			int player_hp_;//プレイヤー体力

			float walk_speed_;//移動速度
			float run_speed_;//移動速度
			float rot_speed_;//回転速度

			float jump_speed_;//ジャンプ速度
			float gravity_;//重力
			bool jump_flag_;//ジャンプ状態記録用フラグ

			bool damage_flag_;//ダメージを受けたことを記録するフラグ
			bool damage_anim_flag_;//ダメージモーションを開始するフラグ

			bool gameover_flag_;//ゲームオーバー状態を判定するフラグ

			//モデルアニメーション用変数
			int anim_attach_index_;//アニメーションアタッチ番号
			int old_anim_attach_index_;//遷移元のアニメーションアタッチ番号
			float anim_rate_;//アニメーションブレンド率
			float anim_total_time_;//アニメーション総再生時間
			float anim_play_time_;//アニメーション再生時間
			bool anim_loop_flag_;//アニメーションをループするかどうか

			//エフェクト用変数
			bool slingshot_flag_;//パチンコ射撃状態かどうかを判定するフラグ
			bool selected_skystar_flag_;//射撃状態から星を選択したかどうか判定するフラグ
			bool shoot_charge_effect_flag_;//射撃溜めエフェクト生成フラグ

			STATUS status_;//プレイヤーの状態保持変数
		};
	}
}