/**
 * @file    Star.h
 * @brief  ステージ内のスター挙動実装クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/01/23
 */

#pragma once
#include "../Object/ObjectBase.h"
#include"StageStarState.h"
#include<unordered_map>

namespace starrynight
{
	namespace star
	{
		/**
		 * @brief  ステージ内のスター挙動実装クラス
		 */
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
			virtual OBJECT_TYPE GetObjectType()const override { return ObjectBase::OBJECT_TYPE::STAGE_STAR; }

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
			 * @brief　 プレイヤーが何番目に取得したスターの数か取得する
			 *
			 * @return   0であればまだ未獲得、1以上であれば獲得済み
			 */
			int GetPlayersStarNum() const { return star_num_; }

			/**
			 * @brief　 プレイヤーが何番目に取得したスターの数か取得する
			 *
			 * @param  _star_num 何番目に所持したスターか
			 */
			void SetPlayersStarNum(const int& _star_num) { star_num_ = _star_num; }

			/**
			 * @brief　 指定した状態に切り替える処理
			 *
			 * @param  _state_name　状態名
			 */
			void ChangeStageStarState(const std::string& _state_name);

			/**
			 * @brief　 待機状態の地面の高さを取得する
			 *
			 * @return   地面の高さ
			 */
			float GetGroundPositionY() const { return ground_position_y_; }

			/**
			 * @brief　 待機状態の地面の高さを設定する
			 *
			 * @param  _position_y　地面の高さ
			 */
			void SetGroundPositionY(const float& _position_y) { ground_position_y_ = _position_y; }

			/**
			 * @brief　 ジャンプの高さを取得する
			 *
			 * @return   ジャンプの高さ
			 */
			float GetJumpHeight() const { return jump_height_; }

			/**
			 * @brief　 ジャンプ速度を取得する
			 *
			 * @return   ジャンプ速度
			 */
			float GetJumpSpeed() const { return jump_speed_; }

			/**
			 * @brief　 ジャンプ速度を設定する
			 *
			 * @param  _jump_speed　ジャンプ速度
			 */
			void SetJumpSpeed(const float& _jump_speed) { jump_speed_ = _jump_speed; }

			/**
			 * @brief　 フィールド内スターの重力を取得する
			 *
			 * @return   重力加速度
			 */
			float GetGravity() const { return gravity_; }

			/**
			 * @brief　 フィールド内スターの回転速度を取得する
			 *
			 * @return   回転速度
			 */
			float GetRotSpeed() const { return rot_speed_; }

			sound::PlayWAVE se_;//効果音

		private:
			/**
			 * @brief　星を描画するかどうかを判定する.
			 *
			 * @return TRUEで描画する
			 */
			int IsStageStarDraw();

			/**
			 * @brief 状態遷移用定数
			 */
			enum class STATUS
			{
				WAIT,
				FOLLOW,
				DIFFUSION
			};

			STATUS status_;//状態遷移用変数

			std::shared_ptr<StageStarState> stagestar_state_;
			std::unordered_map<std::string, std::shared_ptr<StageStarState>> state_map_;//状態変数管理マップ

			int star_num_;//プレイヤーが何番目に取得した星か記録する

			float jump_height_;//ジャンプする高さ
			float jump_speed_;//ジャンプ速度
			float ground_position_y_;//初期のY座標
			float gravity_;//重力
			float rot_speed_;//回転速度

		};
	}
}