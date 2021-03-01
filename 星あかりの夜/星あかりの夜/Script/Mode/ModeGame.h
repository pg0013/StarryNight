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

			camera::Camera camera_;//カメラ
			object::ObjectServer object_server_;//オブジェクト管理サーバ
			effect::EffectServer effect_server_;//エフェクト管理サーバ
			stage::Stage stage_;//ステージ管理クラス
			ui::UIManager ui_;//UI管理クラス

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

			/**
			 * @brief　 ゲームモードを取得する
			 *
			 * @return　ゲームモードインスタンス
			 */
			ModeGame* GetModeGame() { static_cast<mode::ModeGame*>(::mode::ModeServer::GetInstance()->Get("Game")); }

			/**
			 * @brief　 ゲームスコアを追加する
			 *
			 * @param  _score		ゲームのスコア
			 */
			void AddGameScore(int _score) { game_score_ += _score; }

			/**
			 * @brief　 ゲームスコアを取得する
			 *
			 * @return   ゲームスコア
			 */
			int GetGameScore() { return game_score_; }

			/**
			 * @brief　 ステージのフィールドスター総数を設定する
			 *
			 * @param  _stage_star_num	　ステージのフィールドスター総数
			 */
			void SetStageStarNum(int _stage_star_num) { stage_star_num_ = _stage_star_num; }

			/**
			 * @brief　 ステージのフィールドスター総数を取得する
			 *
			 * @return   ステージのフィールドスター総数
			 */
			int GetStageStarNum() { return stage_star_num_; }

			/**
			 * @brief　 ステージの規定スコアを設定する
			 *
			 * @param  _regulations_score	ステージの規定スコア
			 */
			void SetStageRegulationsScore(int _regulations_score) { regulations_score_ = _regulations_score; }

			/**
			 * @brief　 ステージの規定スコアを取得する
			 *
			 * @return   ステージの規定スコア
			 */
			int GetStageRegulationsScore() { return regulations_score_; }

		private:
			bool stop_object_process_;//オブジェクトの処理をストップする
			int player_star_num_;//プレイヤーが取得したスターの数

			int game_score_;//ゲームのスコア
			int stage_star_num_;//ステージのスター総数
			int regulations_score_;//ステージの規定スコア
		};
	}
}