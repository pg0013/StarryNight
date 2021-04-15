/**
 * @file    ModeTutorial.h
 * @brief  チュートリアル画面モードクラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/06
 */

#pragma once
#include "appframe.h"

namespace starrynight
{
	namespace mode
	{
		/**
		 * @brief  チュートリアル画面モードクラス
		 */
		class ModeTutorial :
			public ::mode::ModeBase
		{
		public:
			ModeTutorial();
			~ModeTutorial();

			/**
			 * @brief   初期化処理
			 * モードサーバー登録時に一度だけ実行される
			 *
			 * @return 初期化成功の可否
			 */
			bool Initialize() override;

			/**
			 * @brief   終了処理
			 * モードサーバーから削除時に一度だけ実行される
			 *
			 * @return 終了処理成功の可否
			 */
			bool Terminate() override;

			/**
			 * @brief   演算処理
			 *
			 * @return 演算処理成功の可否
			 */
			bool Process() override;

			/**
			 * @brief   描画処理
			 *
			 * @return 描画処理成功の可否
			 */
			bool Render() override;

		private:
			/**
			 * @brief コントローラ入力処理
			 */
			void Input();

			/**
			 * @brief 次のモードに遷移する処理
			 */
			void NextMode();

			handle background_graph_;//背景画像

			::parameters::Parameters param_;//パラメータ読み込みクラス

			int nextmode_count_;//次のモードへ移行するカウンタ
			bool pushed_flag_;//メニュー選択済みフラグ
		};
	}
}
