/**
 * @file    ModeLoading.h
 * @brief  ローディングモードクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/13
 */

#pragma once
#include "appframe.h"

namespace starrynight
{
	namespace mode
	{
		class ModeLoading :
			public ::mode::ModeBase
		{
		public:
			ModeLoading();
			~ModeLoading();

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

			/**
			 * @brief　 次にモードへ遷移する処理
			 *
			 */
			void SetNextMode();
		private:
			/**
			 * @brief　 次のモードに遷移する処理
			 */
			void NextMode();

			handle tutorial_graph_;//背景画像

			::parameters::Parameters param_;//パラメータ読み込みクラス

			int nextmode_count_;//次のモードへ移行するカウンタ
			bool pushed_flag_;//メニュー選択済みフラグ
		};

	}
}

