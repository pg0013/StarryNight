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
#include"CameraState.h"
#include<unordered_map>

namespace starrynight
{
	namespace camera
	{
		/**
		 * @brief カメラ設定用構造体
		 */
		struct CLIP
		{
			float near_;
			float far_;
		};

		/**
		 * @brief カメラ処理クラス
		 */
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
			 * @brief 演算処理.
			 *
			 */
			void Process();

			/**
			 * @brief 描画処理.
			 *
			 */
			void Render();

		public:
			/**
			 * @brief カメラ座標を取得する
			 *
			 * @return   カメラ座標
			 */
			VECTOR GetPosition() const { return position_; }

			/**
			 * @brief カメラ座標を設定する
			 *
			 * @param  _vector	カメラ座標
			 */
			void SetPosition(const VECTOR& _vector) { position_ = _vector; }

			/**
			 * @brief カメラのターゲット座標を取得する
			 *
			 * @return   ターゲット座標
			 */
			VECTOR GetTarget() const { return target_; }

			/**
			 * @brief カメラのターゲット座標を設定する
			 *
			 * @param  _vector	ターゲット座標
			 */
			void SetTarget(const VECTOR& _vector) { target_ = _vector; }

			/**
			 * @brief targetとcameraの角度を計算して返す
			 *
			 * @return   targetとcameraの角度
			 */
			float GetCameraRad() const;

			/**
			 * @brief 現在のカメラとの距離を返す.
			 *
			 * @return　現在のカメラ位置とターゲットの距離
			 */
			float GetCameraLength() const;

			/**
			 * @brief カメラの移動速度を返す
			 *
			 * @return   移動速度
			 */
			float GetMoveCameraSpeed() const { return move_speed_; }

			/**
			 * @brief 設定したカメラ距離を返す
			 *
			 * @return   カメラ距離
			 */
			float GetCameraDistance() const { return camera_distance_; }

			/**
			 * @brief カメラの回転速度を返す
			 *
			 * @return   回転速度
			 */
			float GetCameraRotSpeed() const { return rot_speed_; }

			/**
			 * @brief カメラのNear,Far設定を取得する
			 *
			 * @return   near,far格納構造体
			 */
			CLIP GetClip() const { return clip_; }

			/**
			 * @brief カメラのNear,Farを設定する
			 *
			 * @param  _near	カメラのNear
			 * @param  _far		カメラのFar
			 */
			void SetClip(const float& _near, const float& _far)
			{
				clip_.near_ = _near;
				clip_.far_ = _far;
			}

			/**
			 * @brief カメラの状態を指定した状態に変更する
			 *
			 * @param  _state_name　遷移するカメラ状態
			 */
			void ChangeCameraState(const std::string& _state_name);

			/**
			 * @brief プレイヤーにカメラが追従する処理
			 *
			 * @param _player_diff　プレイヤーの前フレームとの移動量
			 */

			 /**
			  * @brief プレイヤーにカメラが追従する処理
			  *
			  * @param _player_position 現在のプレイヤー座標
			  * @param _player_old_position 前フレームのプレイヤー座標
			  */
			void PlayerFollowMove(const VECTOR& _player_position, const VECTOR& _player_old_position);

			/**
			 * @brief カメラのデバッグメニューを表示
			 */
			void DrawDebugMenu();

			/**
			 * @brief 状態遷移用変数
			 */
			enum class STATUS
			{
				MOVE,
				SHOOT,
				SKYSTAR
			};

			/**
			 * @brief カメラの状態を設定する
			 *
			 * @param  _status	カメラ状態用変数
			 */
			void SetStatus(const STATUS& _status) { status_ = _status; }

			/**
			 * @brief カメラの状態を取得する
			 *
			 * @return   カメラの状態
			 */
			STATUS GetStatus() const { return status_; }

		private:
			VECTOR position_;//座標
			VECTOR target_;//ターゲット座標
			CLIP clip_;//Near,Far設定構造体

			STATUS status_;//状態遷移用変数
			std::shared_ptr<CameraState> camera_state_;//状態遷移ポインタ
			std::unordered_map<std::string, std::shared_ptr<CameraState>> state_map_;//状態管理マップ

			//カメラパラメータ
			CameraParameters camera_param_;

			float move_speed_;//移動速度
			float rot_speed_;//回転速度
			float camera_distance_;//カメラ距離
		};
	}
}