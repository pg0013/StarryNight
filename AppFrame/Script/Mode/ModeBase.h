/**
 * @file    ModeBase.h
 * @brief  モードの基底クラス
 *
 * @author Takuya Fujisawa
 * @date    20201021
 */

#pragma once
#include<string>

namespace mode
{
	class ModeServer;

	/**
	 * @brief  モードの基底クラス
	 */
	class ModeBase
	{
	public:
		ModeBase();
		virtual ~ModeBase();

		/**
		 * @brief   初期化処理
		 * モードサーバー登録時に一度だけ実行される
		 *
		 * @return 初期化成功の可否
		 */
		virtual bool Initialize();

		/**
		 * @brief   終了処理
		 * モードサーバーから削除時に一度だけ実行される
		 *
		 * @return 終了処理成功の可否
		 */
		virtual bool Terminate();

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

	public:
		/**
		 * @brief   このモードが始まってからのカウントを返す
		 *
		 * @return 経過カウント
		 */
		int GetModeCount() const { return mode_count_; }

		/**
		 * @brief   このモードが始まってからの時間を返す
		 *
		 * @return 経過時間[ms]
		 */
		unsigned long GetModeTime() const { return mode_time_; }

		/**
		 * @brief   前フレームからの経過時間を返す
		 *
		 * @return 前フレームからの経過時間[s]
		 */
		float GetDeltaTime() const { return static_cast<float>(delta_time_ / 1000.0f); }

		/**
		 * @brief   何フレームに何回Process()を呼ぶか設定 default : 1
		 *
		 * @param _frame 何フレームに何回か
		 */
		void SetCallPerFrame(const int& _frame) { callper_frame_ = callper_framecount = _frame; }

		/**
		 * @brief   1回の呼び出しに何回Process()を呼ぶか設定 default : 1
		 *
		 * @param _count 1回の呼び出しに何回Processを回すか
		 */
		void SetCallOfCount(const int& _count) { callof_count_ = _count; }

		/**
		 * @brief   何フレームに何回Process()を呼ぶかを取得
		 *
		 * @return 何フレームに何回Process()を呼ぶか
		 */
		int GetCallPerFrame() const { return callper_frame_; }

		/**
		 * @brief   1回の呼び出しに何回Process()を呼ぶか取得
		 *
		 * @return 1回の呼び出しに何回Processを回すか
		 */
		int GetCallOfCount() const { return callof_count_; }

	private:
		/**
		 * @brief   時間経過させる処理
		 *
		 * @param _nowtime
		 */
		void StepTime(unsigned long _nowtime);

		/**
		 * @brief   カウントを経過させる
		 *
		 */
		void StepCount();

		friend ModeServer;

		std::string modename_;//!<モードサーバー用モード名
		int uid_;//!<モードサーバー登録時に決定するID
		int layer_;//!<表示されるレイヤー番号

		int mode_count_;//!<本モードに入ってからのカウント.0からスタート
		unsigned long mode_time_;//!<本モードに入ってからの経過時間.[ms]
		unsigned long delta_time_;//!<前フレームからの経過時間.[ms]
		unsigned long modebase_time_;//!<このモードが始まった時間.[ms]
		unsigned long pause_base_time_;//!<ポーズ中のベース時間.[ms]
		unsigned long pause_step_time_;//!<ポーズ中の積算時間.[ms] 動作中にこの値をmode_timeに用いる
		unsigned long old_frame_time_;//!<前フレームの時間.[ms]

		int callper_frame_, callper_framecount;//!<何フレームに1回Processを呼ぶか
		int callof_count_;//!<1回の呼び出しに何回Process()を呼ぶか
	};
}