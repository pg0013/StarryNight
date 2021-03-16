/**
 * @file	AMG_Summer_Co_Production_2020\script\ResourceServer.h.
 *
 * @brief 画像や音声ファイルを補完するリソースサーバのクラス
 * 	@details 静的メンバなので、どこでも実行可能
 */

#pragma once
#include	<unordered_map>
#include"../Sound/WAVEReader.h"

namespace resource
{
	/**
	 * @class	ResourceServer
	 *
	 * @brief	画像や音声ファイルを補完するリソースサーバのクラス
	 */
	class ResourceServer
	{
	public:

		/**
		 * @fn	static void ResourceServer::Init();
		 *
		 * @brief	初期化処理
		 *
		 */
		static void	Init();

		/**
		 * @fn	static void ResourceServer::Release();
		 *
		 * @brief	サーバの解放処理
		 */
		static void	Release();

		/**
		 * @brief　 テクスチャのデータを削除する
		 */
		static void	ClearTextureMap();

		/**
		 * @brief　 モデルデータを削除する
		 */
		static void	ClearModelMap();

		/**
		 * @brief　 サウンドデータを削除する
		 */
		static void	ClearSoundMap();

		/**
		 * @fn	static void ResourceServer::ClearMap();
		 *
		 * @brief	すべてのデータを削除
		 */
		static void	ClearMap();

		/**
		 * @fn	static int ResourceServer::LoadDivGraph(const TCHAR* _filename,int _allnum, int _xnum,int _ynum,int _xsize,int _ysize, int* _handle);
		 *
		 * @brief	画像を分割読み込み
		 *
		 * @param			_mode	画像が使用されるモード名（jsonのオブジェクト）
		 * @param 		  	_filename	ファイル名
		 * @param 		  	_allnum  	分割総数
		 * @param 		  	_xnum	 	x方向の分割数
		 * @param 		  	_ynum	 	y方向の分割数
		 * @param 		  	_xsize   	分割画像の画像幅
		 * @param 		  	_ysize   	分割画像の画像の高さ
		 *
		 * @returns	画像読み込みの可否
		 */
		static int LoadDivGraph(std::string _mode, std::string _filename, int _allnum,
			int _xnum, int _ynum, int _xsize, int _ysize);

		/**
		 * @fn	static void ResourceServer::RegisterDivGraph(const TCHAR* _filename, int _allnum, int _xnum, int _ynum, int _xsize, int _ysize);
		 *
		 * @brief	画像を非同期で分割読み込みし、リソースサーバに登録
		 *
		 * @param			_mode	画像が使用されるモード名（jsonのオブジェクト）
		 * @param 		  	_filename	ファイル名
		 * @param 		  	_allnum  	分割総数
		 * @param 		  	_xnum	 	x方向の分割数
		 * @param 		  	_ynum	 	y方向の分割数
		 * @param 		  	_xsize   	分割画像の画像幅
		 * @param 		  	_ysize   	分割画像の画像の高さ
		 */
		static void RegisterDivGraph(std::string _mode, std::string _filename, int _allnum,
			int _xnum, int _ynum, int _xsize, int _ysize,bool _async);

		/**
		 * @fn	static WAVEReader ResourceServer::LoadSound(const TCHAR* _filename);
		 *
		 * @brief	音声ファイルを読み込み
		 *
		 * @param 	_filename	ファイル名
		 *
		 * @returns	音声データ
		 */
		static sound::WAVEReader LoadSound(std::string _filename);

		/**
		 * @brief　3Dモデルをリソースサーバーに登録.
		 *
		 * @param  _foldername	Resource/modelの中のフォルダ名
		 * @param  _filename		モデルのファイル名
		 * @param  _handlename　ハンドルマップに登録するハンドル名
		 * @param  _async			非同期読み込みを行うフラグ
		 */
		static void RegisterModel(const std::string _foldername, const std::string _filename ,const std::string _handlename , bool _async);

		/**
		 * @brief　リソースサーバ内の3Dモデルハンドルの取得.
		 *
		 * @param _filename　ファイル名
		 * @return
		 */
		static int GetModelHandle(const std::string _handlename);

		/**
		 * @brief　 画像ハンドルの取得
		 *
		 * @param  _filename　画像ファイル名
		 * @param  tex_num　分割時の画像番号（デフォルトで0）
		 * @return   画像ハンドル
		 */
		static int GetTexture(const std::string _filename, int tex_num = 0);

	private:
		static std::unordered_map<std::string, sound::WAVEReader> map_sound_;  //!< 音声用マップ
		static std::unordered_map<std::string, std::vector<int>> map_texture_;//!<テクスチャ用マップ
		static std::unordered_map<std::string, int> map_model_;//!<3Dモデル用マップ
	};
}