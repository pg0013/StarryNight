/**
 * @file    Hp_UI.h
 * @brief  hpUI表示クラス
 *
 * @author Takuya Fujisawa
 * @date   2021/03/03
 */

#pragma once
#include"appframe.h"

namespace starrynight
{
	namespace ui
	{
		class Hp_UI
		{
		public:
			Hp_UI();
			~Hp_UI();

			/**
			 * @brief   初期化処理
			 */
			void Initialize();

			/**
			 * @brief   終了処理
			 */
			void Terminate();

			/**
			 * @brief   演算処理
			 */
			void Process();

			/**
			 * @brief   描画処理
			 */
			void Render();

		private:
			/**
			 * @brief　 プレイヤーのHPを更新する
			 */
			void UpdatePlayerHP();

			handle hp_graph_;//画像ハンドル
			int player_hp_;//プレイヤー残りHP

			int interval;//画面右上からの間隔
		};
	}
}
