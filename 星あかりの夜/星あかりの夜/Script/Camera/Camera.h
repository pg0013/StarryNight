/**
 * @file    Camera.h
 * @brief  カメラクラス
 *
 * @author Takuya Fujisawa
 * @date    202012/11
 */
#pragma once
#include"appframe.h"
#include"CameraParameters.h"

namespace starrynight
{
	namespace camera
	{
		struct CLIP
		{
			float near_;
			float far_;
		};

		class Camera
		{
		public:
			Camera();
			~Camera();

			static Camera* instance_;
			static Camera* GetInstance() { return instance_; }

			/**
			 * @brief 初期化処理.
			 *
			 */
			void Initialize();

			/**
			 * @brief 入力処理.
			 *
			 */
			void Input();

			/**
			 * @brief 演算処理.
			 *
			 */
			void Process();

			/**
			 * @brief 描画処理.
			 *
			 */
			void Render();

			VECTOR GetPosition() { return position_; }
			void SetPosition(VECTOR _vector) { position_ = _vector; }

			VECTOR GetTarget() { return target_; }
			void SetTarget(VECTOR _vector) { target_ = _vector; }

			/**
			 * @brief　 targetとcameraの角度を計算して返す
			 * 
			 * @return   targetとcameraの角度
			 */
			float GetCameraRad();

			CLIP GetClip() { return clip_; }
			void SetClip(float _near, float _far)
			{
				clip_.near_ = _near;
				clip_.far_ = _far;
			}

			/**
			 * @brief　 カメラのデバッグメニューを表示
			 */
			void DrawDebugMenu();

		private:
			VECTOR position_;
			VECTOR target_;
			CLIP clip_;

			CameraParameters camera_param_;

			float move_speed_;//移動速度
			float rot_speed_;//回転速度
		};
	}
}