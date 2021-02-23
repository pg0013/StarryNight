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
#include"../Object/ObjectServer.h"
#include"../Effect/EffectServer.h"
#include"../Camera/Camera.h"
#include"../UI/UIManager.h"

namespace starrynight
{
	namespace mode
	{
		class ModeGame :
			public ::mode::ModeBase
		{
		public:
			ModeGame();
			~ModeGame();

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
			 * @brief　 オブジェクトの処理を止めるフラグを設定
			 *
			 * @param  _stopflag　trueでオブジェクトの処理を止める
			 */
			void SetStopObjectProcess(bool _stopflag) { stop_object_process_ = _stopflag; }

			camera::Camera camera_;
			object::ObjectServer object_server_;
			effect::EffectServer effect_server_;
			stage::Stage stage_;
			ui::UIManager ui_;

		public:
			/**
			 * @brief　 プレイヤーが取得したステージスターの数を取得する
			 *
			 * @return   取得したスターの数
			 */
			int GetPlayerStarNum() { return player_star_num_; }

			/**
			 * @brief　 プレイヤーが取得したスターの数を設定する
			 *
			 * @param  _star_num　取得したスターの数
			 */
			void SetPlayerStarNum(int _star_num) { player_star_num_ = _star_num; }

			/**
			 * @brief　 プレイヤーが取得したスターの数を＋１する
			 */
			void AddPlayerStarNum() { player_star_num_++; }

		private:
			bool stop_object_process_;//オブジェクトの処理をストップする
			int player_star_num_;
		};
	}
}