/**
 * @file    ModeMovie.h
 * @brief  オープニングのムービー再生クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/09
 */

#pragma once
#include "appframe.h"

namespace starrynight
{
	namespace mode
	{
		/**
		 * @brief  オープニングのムービー再生クラス
		 */
		class ModeMovie :
			public ::mode::ModeBase
		{
		public:
			ModeMovie();
			~ModeMovie();

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

			handle movie_;//映像ハンドル

			bool pushed_flag_;//ボタンが押されたときのフラグ
			int nextmode_count_;//次のモードに遷移するまでの時間
		};
	}
}