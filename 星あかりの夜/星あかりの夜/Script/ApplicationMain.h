/**
 * @file    ApplicationMain.h
 * @brief  このプロジェクトのメイン処理
 *
 * @author Takuya Fujisawa
 * @date    20201021
 */
#include"appframe.h"

namespace starrynight
{
	/**
	 * @brief  このプロジェクトのメイン処理
	 */
	class ApplicationMain :
		public appframe::ApplicationBase
	{
		typedef appframe::ApplicationBase appbase;

	public:
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
	};
}