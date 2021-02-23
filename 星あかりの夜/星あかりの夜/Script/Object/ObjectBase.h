/**
 * @file    ObjectBase.h
 * @brief  オブジェクトの基底クラス
 *
 * @author Takuya Fujisawa
 * @date    202012/16
 */
#pragma once
#include"appframe.h"

namespace starrynight
{
	namespace object
	{
		class ObjectBase
		{
		public:
			ObjectBase();
			~ObjectBase();

			/**
			 * @brief　オブジェクトの種類検出用変数.
			 */
			enum class OBJECT_TYPE
			{
				PLAYER,ENEMY,STAR,EFFECT
			};

			/**
			 * @brief　オブジェクトの種類を返す.
			 *
			 * @return　オブジェクトの種類
			 */
			virtual OBJECT_TYPE GetObjectType() = 0;

			/**
			 * @brief　 ムーブコンストラクタ
			 * 
			 * @param  _object
			 * @return   
			 */
			ObjectBase(ObjectBase&& _object) noexcept :
				handle_(_object.handle_),
				position_(_object.position_),
				rotation_(_object.rotation_)
			{
			}

			//オペレータ演算子
			ObjectBase& operator=(const ObjectBase& _object);
			ObjectBase& operator=(ObjectBase&& _object) noexcept;

			/**
			 * @fn	virtual void ObjectBase::Initialize();
			 *
			 * @brief	初期化処理
			 *
			 */
			virtual void Initialize();

			/**
			 * @fn	virtual void ObjectBase::Process();
			 *
			 * @brief	毎フレーム呼ばれる演算処理
			 */
			virtual void Process();

			/**
			 * @fn	virtual void ObjectBase::Render();
			 *
			 * @brief	描画処理
			 */
			virtual void Render();

			/**
			 * @brief　オブジェクトの位置を設定する.
			 *
			 * @param _positon 位置(x,y,z)のベクトル構造体
			 */
			inline void SetPosition(VECTOR _positon) { position_ = _positon; MV1SetPosition(handle_, position_); }

			/**
			 * @brief　オブジェクトの位置を返す.
			 *
			 * @return 位置(x,y,z)のベクトル構造体
			 */
			inline VECTOR GetPosition() { return MV1GetPosition(handle_); }

			/**
			 * @brief　オブジェクトの回転角度を設定する.
			 *
			 * @param _rotation　回転角度(x,y,z)[rad]
			 */
			inline void SetRotation(VECTOR _rotation) { rotation_ = _rotation; MV1SetRotationXYZ(handle_, rotation_); }

			/**
			 * @brief　オブジェクトの回転角度を返す.
			 *
			 * @return 回転角度(x,y,z)[rad]
			 */
			inline VECTOR GetRotation() { return MV1GetRotationXYZ(handle_); }

			inline void SetModelHandle(const handle _handle) { handle_ = _handle; }

			/**
			 * @brief	オブジェクトのハンドルを返す.
			 *
			 * @return
			 */
			inline handle GetModelHandle() { return handle_; }

		protected:
			VECTOR position_;
			VECTOR rotation_;
			handle handle_;
		};
	}
}