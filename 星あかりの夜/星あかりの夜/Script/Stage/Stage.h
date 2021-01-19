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
			 * @brief　 ナビメッシュと線分の当たり判定を取得する
			 *
			 * @param  _player_position	プレイヤーの位置
			 * @return   当たり判定
			 */
			MV1_COLL_RESULT_POLY GetHitLineToNaviMesh(VECTOR& _startline,VECTOR& _endline);

			/**
			 * @brief　 オブジェクトと線分の当たり判定を取得する
			 *
			 * @param  _player_position	プレイヤーの位置
			 * @return   当たり判定
			 */
			MV1_COLL_RESULT_POLY GetHitLineToColObject(VECTOR& _startline, VECTOR& _endline);

			/**
			 * @brief	ステージと球の当たり判定を取得する.
			 * 
			 * @param _position	球の中心座標
			 * @param _radius		球の半径
			 * @return						コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitSphereToStage(VECTOR& _position, float _radius);

			/**
			 * @brief	オブジェクトと球の当たり判定を取得する.
			 *
			 * @param _position	球の中心座標
			 * @param _radius		球の半径
			 * @return					コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitSphereToColObject(VECTOR& _position, float _radius);

			/**
			 * @brief ステージとカプセルの当たり判定を取得する.
			 *
			 * @param _position1	カプセルを形成する二点中の一点の座標
			 * @param _position2	カプセルを形成する二点中の一点の座標
			 * @param _radius			半径
			 * @return						コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitCapsuleToStage(VECTOR& _position1, VECTOR& _position2, float _radius);


			/**
			 * @brief オブジェクトとカプセルの当たり判定を取得する.
			 * 
			 * @param _position1	カプセルを形成する二点中の一点の座標
			 * @param _position2	カプセルを形成する二点中の一点の座標
			 * @param _radius			半径
			 * @return						コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitCapsuleToColObject(VECTOR& _position1, VECTOR& _position2, float _radius);


		private:
			StageParameters stage_param_;
			std::vector<handle> stage_handle_;
			std::vector<handle> navimesh_handle_;
		};
	}
}