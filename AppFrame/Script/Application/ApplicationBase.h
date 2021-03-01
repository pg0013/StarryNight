/**
 * @file    ApplicationBase.h
 * @brief  アプリケーションの基底クラス
 *				ゲームごとにオーバーライドして使用する
 *
 * @author Takuya Fujisawa
 * @date   2020/10/25
 */

#include"DxLib.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"EffekseerForDXLib.h"
#include"../Mode/ModeServer.h"

namespace appframe
{
	namespace
	{
		constexpr int SCREEN_WIDTH = 1920;
		constexpr int SCREEN_HEIGHT = 1080;
		constexpr int SCREEN_DEPTH = 32;
	}

	class ApplicationBase
	{
	public:
		ApplicationBase();
		~ApplicationBase();

		/**
		 * @brief   初期化処理
		 *
		 * @param _hinstance　WinMainの第一引数
		 * @return 初期化成功の可否
		 */
		virtual bool Initialize(HINSTANCE _hinstance);

		/**
		 * @brief   終了処理
		 * プログラム終了時の解放処理を行う
		 *
		 * @return 終了処理成功の可否
		 */
		virtual bool Terminate();

		/**
		 * @brief   入力処理
		 * ゲームコントローラーの入力を検知する
		 *
		 * @return 入力処理成功の可否
		 */
		virtual bool Input();

		/**
		 * @brief   演算処理
		 *
		 * @return 演算処理成功の可否
		 */
		virtual bool Process();

		/**
		 * @brief   描画処理
		 *
		 * @return 描画処理成功の可否
		 */
		virtual bool Render();

		/**
		 * @brief   ウィンドウモードでアプリケーションを実行するかを決定
		 *
		 * @param _window_mode　TRUEでウィンドウモードで実行
		 *											FALSEでフルスクリーンモードで実行
		 */
		virtual void AppWindowed(bool _window_mode);

		/**
		 * @brief   デバッグログを実行時に出力するかどうか
		 *
		 * @param _window_mode　TRUEでdebug.log.txtを出力する
		 */
		virtual void OutputDebugLog(bool _window_mode);

		/**
		 * @brief   インスタンスを取得
		 *
		 * @return ApplicationBaseのインスタンス
		 */
		static ApplicationBase* GetInstance() { return appbase_instance_; }

		/**
		 * @brief　 キーボードとコントローラのkey入力を返す
		 *
		 * @return   コントローラとキー入力
		 */
		virtual int GetKey() { return key_; }

		/**
		 * @brief　 キーボードとコントローラのトリガー入力を返す
		 *
		 * @return   コントローラとキーのトリガー入力
		 */
		virtual int GetTriggerKey() { return trigger_key_; }

		/**
		 * @brief　 XINPUTコントローラの入力を返す
		 *
		 * @return   XINPUTコントローラの入力
		 */
		virtual XINPUT_STATE GetXInputState() { return x_input_; }

		/**
		 * @brief　 DIRECTINPUTコントローラの入力を返す
		 *
		 * @return   DINPUTコントローラの入力を返す
		 */
		virtual DINPUT_JOYSTATE GetDInputState() { return d_input_; }

		/**
		 * @brief	現在のFPSを算出する処理.
		 */
		void CalculateFPS();

	protected:
		static ApplicationBase* appbase_instance_;

		int key_; //!<コントローラーの入力
		int trigger_key_; //!<コントローラーのトリガー入力
		XINPUT_STATE x_input_;//!<XINPUT用コントローラ入力
		DINPUT_JOYSTATE d_input_;//!<DIRECTINPUT用コントローラ入力

		mode::ModeServer* mode_server_;//!<モード管理用サーバ

		//FPS計測用変数
		LONGLONG now_time_, fps_check_time_;
		int fps_count_;
		int fps_;
	};
}