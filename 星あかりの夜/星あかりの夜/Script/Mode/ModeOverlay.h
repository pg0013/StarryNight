/**
 * @file    ModeOverlay.h
 * @brief	画面遷移モードクラス
 * 				キャプチャかフェードかを指定して画面遷移を行う
 * 				キャプチャでは、表示画面の上からオーバーレイ表示
 * 				フェードでは、画面を暗転させる
 *
 * @author Takuya Fujisawa
 * @date   2021/03/03
 */

#pragma once
#include "appframe.h"

namespace
{
	constexpr int FADE_IN = 0;
	constexpr int FADE_OUT = 1;
}

namespace starrynight
{
	namespace mode
	{
		/**
		 * @brief	画面遷移モードクラス
		 */
		class ModeOverlay :
			public ::mode::ModeBase
		{
		public:
			ModeOverlay();
			~ModeOverlay();

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

			/**
			 * @brief	現在表示されているモードをキャプチャして、画面遷移を行う
			 *
			 * @param 	_fade_count	フェード時間
			 */
			void Capture(int _fade_count);

			/**
			 * @brief	フェードイン : 暗転から明るくなる
			 * 				フェードアウト : 明るい画面から徐々に暗転
			 *
			 * @param 	_fadecount	フェード時間
			 * @param 	_fade_type 	フェードイン : 0
			 * 									フェードアウト : 1
			 */
			void Fade(int _fade_count, int _fade_type);

		private:
			int background_graph_;//暗転用の背景
			int fade_count_;//フェード時間
			int all_fade_count_;//フェード終了時間
			int fade_type_;//フェードの種類
		};
	}
}
