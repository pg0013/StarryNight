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

namespace
{
	constexpr int GRASS = 0;
	constexpr int ROCK = 1;
	constexpr int WOOD = 2;
}

namespace starrynight
{
	namespace stage
	{
		/**
		 * @brief  ステージ管理クラス
		 */
		class Stage
		{
		public:
			Stage();
			~Stage();

			static Stage* instance_;

			/**
			 * @brief インスタンスを返す
			 *
			 * @return   インスタンス
			 */
			static Stage* GetInstance() { return instance_; }

			/**
			 * @brief　初期化処理.
			 *
			 * @param  _stage_name	ステージ名
			 */
			void Initialize(const std::string& _stage_name);

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
			 * @brief ナビメッシュと線分の当たり判定を取得する
			 *
			 * @param  _startline　	線分の開始地点
			 * @param  _endline		線分の終了地点
			 * @return   当たり判定
			 */
			MV1_COLL_RESULT_POLY GetHitLineToFloor(const VECTOR& _startline, const VECTOR& _endline) const;

			/**
			 * @brief オブジェクトと線分の当たり判定を取得する
			 *
			 * @param  _startline　	線分の開始地点
			 * @param  _endline		線分の終了地点
			 * @return   当たり判定
			 */
			MV1_COLL_RESULT_POLY GetHitLineToWall(const VECTOR& _startline, const VECTOR& _endline) const;

			/**
			 * @brief	ステージと球の当たり判定を取得する.
			 *
			 * @param _position	球の中心座標
			 * @param _radius		球の半径
			 * @return						コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitSphereToFloor(const VECTOR& _position, const float& _radius) const;

			/**
			 * @brief	オブジェクトと球の当たり判定を取得する.
			 *
			 * @param _position	球の中心座標
			 * @param _radius		球の半径
			 * @return					コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitSphereToWall(const VECTOR& _position, const float& _radius) const;

			/**
			 * @brief ステージとカプセルの当たり判定を取得する.
			 *
			 * @param _position1	カプセルを形成する二点中の一点の座標
			 * @param _position2	カプセルを形成する二点中の一点の座標
			 * @param _radius			半径
			 * @return						コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitCapsuleToFloor(const VECTOR& _position1, const VECTOR& _position2, const float& _radius) const;

			/**
			 * @brief オブジェクトとカプセルの当たり判定を取得する.
			 *
			 * @param _position1	カプセルを形成する二点中の一点の座標
			 * @param _position2	カプセルを形成する二点中の一点の座標
			 * @param _radius			半径
			 * @return						コリジョン結果代入用ポリゴン配列
			 */
			MV1_COLL_RESULT_POLY_DIM GetHitCapsuleToWall(const VECTOR& _position1, const VECTOR& _position2, const float& _radius) const;

			/**
			 * @brief 射撃ポイントの床と当たり判定を取得する
			 *
			 * @param  _startline　	線分の開始地点
			 * @param  _endline		線分の終了地点
			 * @return   当たり判定構造体
			 */
			MV1_COLL_RESULT_POLY GetHitLineToShootPoint(const VECTOR& _startline, const VECTOR& _endline) const;

			/**
			 * @brief 床と線分の当たり判定をし、判定があれば床の材質を返す
			 *
			 * @param  _startline　	線分の開始地点
			 * @param  _endline		線分の終了地点
			 * @return   床の材質
			 *				GRASS = 0;
			 *				ROCK = 1;
			 *				WOOD = 2;
			 *				当たり判定がない　-1
			 */
			int GetHitFloorType(const VECTOR& _startline, const VECTOR& _endline) const;
		private:
			/**
			 * @brief ステージモデルを初期化、配置する
			 *
			 */
			void InitStageModel();

			/**
			 * @brief ステージスターを初期化、配置する
			 *
			 */
			void InitStageStar();

			/**
			 * @brief 星座を初期化、配置する
			 *
			 */
			void InitSkyStar();

			/**
			 * @brief 敵を初期化、配置する
			 *
			 */
			void InitEnemy();

			/**
			 * @brief ステージのエフェクトを初期化、配置する
			 *
			 */
			void InitStageEffect();

			/**
			 * @brief ファイル名から床の材質を返す
			 *
			 * @param  _filename　モデルのファイル名
			 * @return   材質のタイプ
			 */
			int CheckFloorType(std::string& _filename);

			StageParameters stage_param_;//ステージパラメータ管理クラス
			star::StarParameter star_param_;//スターパラメータ管理クラス
			star::SkyStarParameter skystar_param_;//星座パラメータ管理クラス
			enemy::EnemyParameters enemy_param_;//エネミーパラメータ管理クラス

			std::vector<handle> stage_handle_;//ステージモデルのハンドル管理コンテナ
			std::vector<handle> navimesh_handle_;//ステージの当たり判定管理コンテナ
			std::unordered_map<handle, int> map_floortype_;//床の材質を格納するコンテナ

			handle skysphere_;//天球用ハンドル
			handle shootpoint_;//射撃ポイントのハンドル
		};
	}
}