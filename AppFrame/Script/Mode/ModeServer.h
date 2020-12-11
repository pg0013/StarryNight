/** 
 * @file    ModeServer.h
 * @brief  
 * 
 * @author Takuya Fujisawa
 * @date    2020/10/21
 */
#pragma once
#include<list>
#include"ModeBase.h"

namespace mode
{
	class ModeServer
	{
	public:
		ModeServer();
		virtual ~ModeServer();

		static ModeServer* modeserver_instance_;
		static ModeServer* GetInstance() { return (ModeServer*)modeserver_instance_; }

		/**
		 * @brief   モードサーバーに追加予約する
		 * モードサーバーに追加されるのは1フレーム後
		 * 
		 * @param _mode　追加するモード
		 * @param _layer　表示するレイヤー
		 * @param _name　モード名
		 * @return
		 */
		int Add(ModeBase* _mode, int _layer, const char* _name);

		/**
		 * @brief   削除予約をする
		 * モードサーバーから削除されるのは1フレーム後
		 * 
		 * @param _mode　削除するモード
		 * @return 　0
		 */
		int Del(ModeBase* _mode);

		/**
		 * @brief	uidから指定したモードを取得
		 *
		 * @param 	_uid	指定モードのuid
		 *
		 * @return		指定したuidを持つモード
		 */
		ModeBase* Get(int _uid);

		/**
		 * @brief	モード名からモードを取得
		 *
		 * @param 	_name	指定モード名
		 *
		 * @returns	指定した名前のモード
		 */
		ModeBase* Get(const char* _name);

		/**
		 * @brief	uidを取得
		 *
		 * @param [in,out]	_mode	モードポインタ
		 *
		 * @returns	指定したモードのuid
		 */
		int GetId(ModeBase* _mode);

		/**
		 * @brief	uidを取得
		 *
		 * @param 	_name	モード名
		 *
		 * @returns	指定したモードのuid
		 */
		int GetId(const char* _name);

		/**
		 * @brief	モード名を取得
		 *
		 * @param [in,out]	_mode	モードポインタ
		 *
		 * @returns	モード名
		 */
		const char* GetName(ModeBase* _mode);

		/**
		 * @brief	モード名を取得
		 *
		 * @param 	_uid	The UID.
		 *
		 * @returns	モード名.
		 */
		const char* GetName(int _uid);

		/**
		 * @brief	モードサーバーのモードをすべて削除
		 *
		 */
		void Clear();

		/**
		 * @brief	Layer top
		 *
		 * @returns	An int.
		 */
		int	 LayerTop() { return INT32_MAX; }

		/**
		 * @brief	レイヤー順にソートする
		 * @param 	_x	ModeBase
		 * @param 	_y	ModeBase
		 *
		 * @returns	_xが_yよりレイヤーが小さいかどうか
		 */
		static bool ModeSort(const ModeBase* _x, const ModeBase* _y)
		{
			return _x->layer_ < _y->layer_;
		}

		/**
		 * @brief	プロセスを回すための初期化
		 *
		 * @returns	An int.
		 */
		int ProcessInit();

		/**
		 * @brief	レイヤーの上の方から処理
		 *
		 * @returns	An int.
		 */
		int Process();

		/**
		 * @brief	プロセスを回した後の後始末
		 *
		 * @returns	An int.
		 */
		int ProcessFinish();

		/**
		 * @brief	描画を回すための初期化
		 *
		 * @returns	An int.
		 */
		int RenderInit();

		/**
		 * @brief	レイヤーの下の方から処理
		 *
		 * @returns	An int.
		 */
		int Render();

		/**
		 * @brief	描画を回した後の後始末
		 *
		 * @returns	An int.
		 */
		int RenderFinish();

		/**
		 * @brief	現Processで、今処理しているレイヤーより下のレイヤーは、処理を呼ばない
		 *
		 * @returns	An int.
		 */
		int SkipProcessUnderLayer();

		/**
		 * @brief	現Processで、今処理しているレイヤーより下のレイヤーは、描画を呼ばない
		 *
		 * @returns	An int.
		 */
		int SkipRenderUnderLayer();

		/**
		 * @brief	現Processで、今処理しているレイヤーより下のレイヤーは、時間経過を止める
		 *
		 * @returns	An int.
		 */
		int PauseProcessUnderLayer();

	private:

		/**
		 * @brief	削除＆delete
		 *
		 * @param [in,out]	mode	モードベースポインタ
		 *
		 * @returns	An int.
		 */
		int Release(ModeBase* mode);

		/**
		 * @brief	削除予約されているか？
		 *
		 * @param [in,out]	mode	モードベースポインタ
		 *
		 * @returns	削除予約の可否
		 */
		bool IsDelRegist(ModeBase* mode);

		/**
		 * @brief	リストにあるか？
		 *
		 * @param [in,out]	mode	モードベースポインタ
		 *
		 * @returns	追加予約の可否
		 */
		bool IsAdd(ModeBase* mode);

	private:
		std::list<ModeBase*> mode_list_; //!< モードリスト
		int	 uid_count_; //!< uidカウンタ
		std::list<ModeBase*> addmode_list_;  //!< 追加予約
		std::list<ModeBase*> delmode_list_;  //!< 削除予約

		ModeBase* now_mode_; //!< 現在呼び出し中のモード
		ModeBase* skip_processmode_; //!< このモードより下はProcessを呼ばない
		ModeBase* skip_rendermode_;	//!< このモードより下はRenderを呼ばない
		ModeBase* pause_processmode_;	//!< このモードより下はProcess時に時間経過をさせない
	};
}