/**
 * @file    Stage.h
 * @brief  ステージ管理クラス
 *
 * @author Takuya Fujisawa
 * @date    202012/16
 */

#pragma once
#include"appframe.h"
#include"StageParameters.h"
#include<vector>

namespace starrynight
{
	namespace stage
	{
		class Stage
		{
		public:
			Stage();
			~Stage();

			static Stage* instance_;
			static Stage* GetInstance() { return instance_; }

			/**
			 * @brief　初期化処理.
			 */
			void Initialize();

			/**
			 * @brief　演算処理.
			 */
			void Process();

			/**
			 * @brief　描画処理.
			 */
			void Render();

			/**
			 * @brief　ステージのモデルハンドルを削除.
			 */
			void ClearHandle();

			/**
			 * @brief　 ナビメッシュとの当たり判定を取得する
			 *
			 * @param  _player_position	プレイヤーの位置
			 * @return   当たり判定
			 */
			MV1_COLL_RESULT_POLY GetHitToNaviMesh(VECTOR& _startline,VECTOR& _endline);

			/**
			 * @brief　 オブジェクトとの当たり判定を取得する
			 *
			 * @param  _player_position	プレイヤーの位置
			 * @return   当たり判定
			 */
			MV1_COLL_RESULT_POLY GetHitToColObject(VECTOR& _startline, VECTOR& _endline);

		private:
			StageParameters stage_param_;
			std::vector<handle> stage_handle_;
			std::vector<handle> navimesh_handle_;
		};
	}
}