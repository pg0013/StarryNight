/**
 * @file    ModeGame.h
 * @brief  ゲームモードの定義
 *
 * @author Takuya Fujisawa
 * @date    202012/09
 */
#pragma once
#include "appframe.h"
#include"../Stage/Stage.h"

namespace starrynight
{
    class ModeGame :
        public mode::ModeBase
    {
    public:
		ModeGame();
		~ModeGame();

		/**
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

		camera::Camera camera_;
		object::ObjectServer object_server_;
		parameters::Parameters game_param_;
		Stage stage_;
	private:
		handle player_;
    };
}