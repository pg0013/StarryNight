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
#include"../Star/StarParameter.h"
#include"../Star/SkyStarParameter.h"
#include"../Enemy/EnemyParameters.h"
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
			 * @param  _startline　	線分の開始地点
			 * @param  _endline		線分の終了地点
			 * @return   当たり判定
			 */
			MV1_COLL_RESULT_POLY GetHitLineToFloor(VECTOR& _startline, VECTOR& _endline);

			/**
			 * @brief　 オブジェクトと線分の当たり判定を取得する
			 *
			 * @param  _startline　	線分の開始地点
			 * @param  _endline		線分の終了地点
			 * @return   当たり判定
			 */
			MV1_COLL_RESULT_POLY GetHitLineToWall(VECTOR& _startline, VECTOR& _endline);

			/**
			 * @brief	ステージと球の当たり判定を取得する.
			 *
			 * @param _position	球の中心座標
			 * @param _radius		球の半径
			 * @return						コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitSphereToFloor(VECTOR& _position, float _radius);

			/**
			 * @brief	オブジェクトと球の当たり判定を取得する.
			 *
			 * @param _position	球の中心座標
			 * @param _radius		球の半径
			 * @return					コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitSphereToWall(VECTOR& _position, float _radius);

			/**
			 * @brief ステージとカプセルの当たり判定を取得する.
			 *
			 * @param _position1	カプセルを形成する二点中の一点の座標
			 * @param _position2	カプセルを形成する二点中の一点の座標
			 * @param _radius			半径
			 * @return						コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitCapsuleToFloor(VECTOR& _position1, VECTOR& _position2, float _radius);

			/**
			 * @brief オブジェクトとカプセルの当たり判定を取得する.
			 *
			 * @param _position1	カプセルを形成する二点中の一点の座標
			 * @param _position2	カプセルを形成する二点中の一点の座標
			 * @param _radius			半径
			 * @return						コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitCapsuleToWall(VECTOR& _position1, VECTOR& _position2, float _radius);

			/**
			 * @brief　 射撃ポイントの床と当たり判定を取得する
			 *
			 * @param  _startline　	線分の開始地点
			 * @param  _endline		線分の終了地点
			 * @return   当たり判定構造体
			 */
			MV1_COLL_RESULT_POLY GetHitLineToShootPoint(VECTOR& _startline, VECTOR& _endline);

		private:
			StageParameters stage_param_;//ステージパラメータ管理クラス
			star::StarParameter star_param_;//スターパラメータ管理クラス
			star::SkyStarParameter skystar_param_;//星座パラメータ管理クラス
			enemy::EnemyParameters enemy_param_;//エネミーパラメータ管理クラス

			std::vector<handle> stage_handle_;//ステージモデルのハンドル管理コンテナ
			std::vector<handle> navimesh_handle_;//ステージの当たり判定管理コンテナ

			handle skysphere_;//天球用ハンドル
			handle shootpoint_;//射撃ポイントのハンドル
		};
	}
}