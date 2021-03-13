/**
 * @file    EffectBase.h
 * @brief  エフェクトの基底クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/02/09
 */

#pragma once
#include"appframe.h"

namespace starrynight
{
	namespace effect
	{
		class EffectBase
		{
		public:
			EffectBase();
			virtual ~EffectBase();

			/**
			 * @brief　 初期化処理
			 */
			virtual void Initialize();

			/**
			 * @brief　毎フレーム呼ばれる演算処理
			 */
			virtual void Process();

			/**
			 * @brief　 描画処理
			 */
			virtual void Render();

			/**
			 * @brief　オブジェクトの位置を設定する.
			 *
			 * @param _positon 位置(x,y,z)のベクトル構造体
			 */
			inline void SetPosition(VECTOR _positon) { position_ = _positon; }

			/**
			 * @brief　オブジェクトの位置を返す.
			 *
			 * @return 位置(x,y,z)のベクトル構造体
			 */
			inline VECTOR GetPosition() { return position_; }

			/**
			 * @brief　オブジェクトの回転角度を設定する.
			 *
			 * @param _rotation　回転角度(x,y,z)[rad]
			 */
			inline void SetRotation(VECTOR _rotation) { rotation_ = _rotation; }

			/**
			 * @brief　オブジェクトの回転角度を返す.
			 *
			 * @return 回転角度(x,y,z)[rad]
			 */
			inline VECTOR GetRotation() { return rotation_; }

			/**
			 * @brief　 エフェクトを再生する
			 */
			void PlayEffect();

			/**
			 * @brief　 再生中のエフェクト座標を更新する
			 */
			void SetPlayingEffectPosition();

			/**
			 * @brief　 再生中のエフェクト回転角度を更新する
			 */
			void SetPlayingEffectRotation();

		protected:
			/**
			 * @brief　 DXライブラリのベクトルをEffekseerのベクトルに変換する
			 *
			 * @param  _vector	DXライブラリのベクトル構造体
			 * @return   Effekseerのベクトル構造体
			 */
			Effekseer::Vector3D DXLibtoEffekseer(VECTOR _vector)
			{
				return Effekseer::Vector3D(_vector.x, _vector.y, _vector.z);
			}

			VECTOR position_;//座標ベクトル
			VECTOR rotation_;//回転ベクトル

			handle effect_resource_;//エフェクトファイルをロードするハンドル
			handle playing_effect_;//ロードしたファイルからエフェクトを生成したもの

			int effect_frame_;//エフェクトの総再生時間
		};
	}
}
