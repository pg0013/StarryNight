/**
 * @file    SkyStar.h
 * @brief  射撃モードで射る星のクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/27
 */

#pragma once
#include"appframe.h"
#include "../Object/ObjectBase.h"
#include"SkyStarParameter.h"

namespace starrynight
{
	namespace star
	{
		class SkyStar :
			public object::ObjectBase
		{
		public:
			/**
			 * @brief　 星座モデルを生成する
			 *
			 * @param  _name　星座名
			 */
			SkyStar(const std::string _name);
			~SkyStar();

			/**
			 * @brief オブジェクトの種類を返す.
			 *
			 * @param ObjectBase::OBJECT_TYPE::PLAYER
			 */
			virtual OBJECT_TYPE GetObjectType() override { return ObjectBase::OBJECT_TYPE::SKY_STAR; }

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
			 * @brief　 射撃時の星座との当たり判定を取得する
			 *
			 * @param  _start　線分の開始地点
			 * @param  _end	線分の終了地点
			 * @return   当たり判定結果構造体
			 */
			MV1_COLL_RESULT_POLY GetHitToSkyStar(VECTOR _start, VECTOR _end);
		private:
			/**
			 * @brief　 モデルがプレイヤーの方向を向くようにrotationを設定する
			 */
			void SetRotationToPlayer();

			std::string zodiac_name_;//星座名
		};
	}
}
