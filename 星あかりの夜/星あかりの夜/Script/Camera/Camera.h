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
		//カメラ設定用構造体
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

			/**
			 * @brief　 カメラ座標を取得する
			 *
			 * @return   カメラ座標
			 */
			VECTOR GetPosition() { return position_; }

			/**
			 * @brief　 カメラ座標を設定する
			 *
			 * @param  _vector	カメラ座標
			 */
			void SetPosition(VECTOR _vector) { position_ = _vector; }

			/**
			 * @brief　 カメラのターゲット座標を取得する
			 *
			 * @return   ターゲット座標
			 */
			VECTOR GetTarget() { return target_; }

			/**
			 * @brief　 カメラのターゲット座標を設定する
			 *
			 * @param  _vector	ターゲット座標
			 */
			void SetTarget(VECTOR _vector) { target_ = _vector; }

			/**
			 * @brief　 targetとcameraの角度を計算して返す
			 *
			 * @return   targetとcameraの角度
			 */
			float GetCameraRad();

			/**
			 * @brief カメラとの距離を返す.
			 *
			 * @return
			 */
			float GetCameraLength();

			/**
			 * @brief　 カメラのNear,Far設定を取得する
			 *
			 * @return   near,far格納構造体
			 */
			CLIP GetClip() { return clip_; }

			/**
			 * @brief　 カメラのNear,Farを設定する
			 *
			 * @param  _near	カメラのNear
			 * @param  _far		カメラのFar
			 */
			void SetClip(float _near, float _far)
			{
				clip_.near_ = _near;
				clip_.far_ = _far;
			}

			/**
			 * @brief　 カメラのデバッグメニューを表示
			 */
			void DrawDebugMenu();

		public:
			/**
			 * @brief　移動時のカメラ制御処理
			 *
			 */
			void MoveCamera();

			/**
			 * @brief　 射撃時のカメラ位置初期化処理
			 *
			 */
			void ShootCameraInit();

			/**
			 * @brief　 射撃時のカメラ制御処理
			 *
			 */
			void ShootCamera();

			/**
			 * @brief 状態遷移用変数
			 */
			enum class STATUS
			{
				MOVE,
				SHOOT
			};

			/**
			 * @brief　 カメラの状態を設定する
			 *
			 * @param  _status	カメラ状態用変数
			 */
			void SetStatus(STATUS _status) { status_ = _status; }

			/**
			 * @brief　 カメラの状態を取得する
			 *
			 * @return   カメラの状態
			 */
			STATUS GetStatus() { return status_; }

		private:
			VECTOR position_;//座標
			VECTOR target_;//ターゲット座標
			CLIP clip_;//Near,Far設定構造体

			STATUS status_;//状態遷移用変数

			//カメラパラメータ
			CameraParameters camera_param_;

			float move_speed_;//移動速度
			float rot_speed_;//回転速度
		};
	}
}