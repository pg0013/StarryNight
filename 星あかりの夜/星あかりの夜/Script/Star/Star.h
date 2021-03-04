/**
 * @file    Star.h
 * @brief  ステージ内のスター挙動実装クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/01/23
 */

#pragma once
#include "../Object/ObjectBase.h"
#include<queue>

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
			virtual OBJECT_TYPE GetObjectType() override { return ObjectBase::OBJECT_TYPE::STAGE_STAR; }

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
			 * @brief　 プレイヤーが何番目に取得したスターの数か取得する
			 *
			 * @return   0であればまだ未獲得、1以上であれば獲得済み
			 */
			int GetPlayersStarNum() { return star_num_; }
		private:
			/**
			 * @brief 待機処理.
			 */
			void Wait();

			/**
			 * @brief　 単振動挙動処理
			 */
			void Vibration();

			/**
			 * @brief　プレイヤーの後ろをついてくる処理.
			 */
			void Follow();

			/**
			 * @brief　プレイヤーの元から散らばる処理.
			 */
			void Diffusion();

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
				DIFFUSION,
				_EOT_
			};

			STATUS status_;//状態遷移用変数

			std::queue<VECTOR> player_pos_history_;//プレイヤーの位置ベクトル記録用コンテナ
			VECTOR old_player_position_;//前フレームのプレイヤーの位置ベクトルを記録する

			float jump_height_;//ジャンプする高さ
			float jump_speed_;//ジャンプ速度
			float ground_position_y_;//初期のY座標
			float gravity_;//重力
			float rot_speed_;//待機時回転速度

			int star_num_;//プレイヤーが何番目に取得したスターかを記録
			float follow_interval_;//プレイヤーやひとつ前の星との間隔
		};
	}
}