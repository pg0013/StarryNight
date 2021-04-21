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
#include"PlayerState.h"
#include"PlayerParameters.h"
#include<unordered_map>

namespace starrynight
{
	namespace player
	{
		/**
		 * @brief  ゲームモード内のプレイヤークラス
		 */
		class Player :
			public object::ObjectBase
		{
		public:
			Player(const std::string& _stage_name);
			~Player();

			/**
			 * @brief オブジェクトの種類を返す.
			 *
			 * @param ObjectBase::OBJECT_TYPE::PLAYER
			 */
			OBJECT_TYPE GetObjectType() const override { return ObjectBase::OBJECT_TYPE::PLAYER; }

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
			 * @brief プレイヤーの状態を指定した状態に変更する
			 *
			 * @param  _state_name　指定する状態名
			 */
			void ChangePlayerState(const std::string& _state_name);

			/**
			 * @brief プレイヤーが踏んでいる床の材質を設定する
			 *
			 * @param  _floor_type　床の材質
			 */
			void SetFloorType(const int& _floor_type) { floor_type_ = _floor_type; }

			/**
			 * @brief ダメージを受けたことを記録するフラグを設定する
			 *
			 * @param  _flag　ダメージを受けたフラグ
			 */
			void SetDamageFlag(const bool& _flag) { damage_flag_ = _flag; }

			/**
			 * @brief ダメージを受けたことを記録するフラグを返す
			 *
			 * @return   trueでダメージを受けた
			 */
			bool GetDamageFlag() const { return damage_flag_; }

			/**
			 * @brief	射撃状態かどうかを判定するフラグを取得する.
			 *
			 * @return	 trueで射撃状態中
			 */
			bool GetPlayerSlingShotStatus() const { return slingshot_flag_; }

			/**
			 * @brief 射撃状態かどうかを設定する
			 *
			 * @param _slingshot_flag trueで射撃状態中
			 */
			void SetPlayerSlingShotStatus(const bool& _slingshot_flag) { slingshot_flag_ = _slingshot_flag; }

			/**
			 * @brief 射撃構え時に星が選択されたかどうか取得するフラグ
			 *
			 * @return   trueで星選択済み
			 */
			bool GetSelectedStarFlag() const { return selected_skystar_flag_; }

			/**
			 * @brief 射撃構え時に星が選択されたかどうかのフラグを設定する
			 *
			 * @param _selected_skystar_flag trueで星選択済み
			 */
			void SetSelectedStarFlag(const bool& _selected_skystar_flag) { selected_skystar_flag_ = _selected_skystar_flag; }

			/**
			 * @brief アニメーション再生時間を取得する
			 *
			 * @return   アニメーション再生時間
			 */
			float GetAnimPlayTime() const { return anim_play_time_; }

			/**
			 * @brief アニメーションの総再生時間を取得する
			 *
			 * @return   アニメーション再生時間
			 */
			float GetAnimTotalTime() const { return anim_total_time_; }

			/**
			 * @brief プレイヤーの残りHPを返す
			 *
			 * @return   HP
			 */
			int GetPlayerHP() const { return player_hp_; }

			/**
			 * @brief プレイヤーのHPを加算する
			 *
			 * @param  _addhp　加えるHP数
			 */
			void AddPlayerHP(const int& _addhp) { player_hp_ += _addhp; }

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
			 * @brief プレイヤーの状態を取得
			 *
			 * @return   プレイヤー状態
			 */
			STATUS GetPlayerStatus() const { return status_; }

			/**
			 * @brief プレイヤーの状態を設定する
			 *
			 * @param  _status　プレイヤー状態
			 */
			void SetPlayerStatus(const Player::STATUS& _status) { status_ = _status; }

			//プレイヤーパラメータ保持クラス
			PlayerParameters player_param_;

			//プレイヤー効果音
			sound::Sound se_;

		private:
			/**
			 * @brief ステージの外に飛び出たときの処理
			 *
			 */
			void OutOfStage();

			/**
			 * @brief プレイヤーのアクション切り替えを行う処理
			 *
			 */
			void SwitchPlayerAction();

			/**
			 * @brief アニメーション切り替え関数
			 *
			 * @param  _old_status	遷移元の状態
			 */
			void SwitchPlayerAnimation(const STATUS& _old_status);

			/**
			 * @brief 別のアニメーションに移行するときにアニメーションをブレンドする処理
			 */
			void PlayerAnimationBlend();

			/**
			 * @brief ステータスからサウンドを設定する
			 */
			void SwitchPlayerSound();

		private:
			//プレイヤー状態ポインタ
			std::shared_ptr<PlayerState> player_state_;
			//プレイヤー状態保持マップ
			std::unordered_map<std::string, std::shared_ptr<PlayerState>> state_map_;

			int player_hp_;//プレイヤー体力

			int floor_type_;//床の材質

			bool damage_flag_;//ダメージを受けたことを記録するフラグ

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

			STATUS status_;//プレイヤーの状態保持変数
		};
	}
}