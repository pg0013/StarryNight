/**
 * @file	AMG_Summer_Co_Production_2020\script\ResourceServer.cpp.
 *
 * @brief	画像や音声ファイルを補完するリソースサーバのクラス
 */

#include "DxLib.h"
#include "ResourceServer.h"
#include"../Utility/Utility.h"
namespace resource
{
	// 静的メンバ実体
	std::unordered_map<std::string, sound::WAVEReader> ResourceServer::map_sound_;
	std::unordered_map < std::string, std::vector<int>> ResourceServer::map_texture_;
	std::unordered_map<std::string, int> ResourceServer::map_model_;

	void ResourceServer::Init()
	{
		map_texture_.clear();
		map_sound_.clear();
		map_model_.clear();
	}

	void ResourceServer::Release()
	{
		ClearMap();
	}

	void ResourceServer::ClearMap()
	{
		for (auto iter : map_texture_)
		{
			for (auto handle : iter.second)
				DeleteGraph(handle);
		}

		for (auto iter : map_model_)
		{
			MV1DeleteModel(iter.second);
		}

		for (auto iter = map_sound_.begin(); iter != map_sound_.end(); iter++)
		{
			free(iter->second.GetpBuffer());
		}

		map_texture_.clear();
		map_model_.clear();
		map_sound_.clear();
	}

	int ResourceServer::LoadDivGraph(std::string _mode, std::string _filename, int _allnum,
		int _xnum, int _ynum, int _xsize, int _ysize)
	{
		// キーの検索
		auto iter = map_texture_.find(_filename);
		if (iter != map_texture_.end())
		{
			//登録済みなので終了
			return 0;
		}

		//キーがなかったらメモリを作成
		std::vector<int> tmp_handle(_allnum);
		std::string filename = "Resource/graphic/" + _mode + "/" + _filename;
		int error = ::LoadDivGraph(filename.c_str(), _allnum, _xnum, _ynum, _xsize, _ysize, tmp_handle.data());
		if (error != 0)
		{
			printfDx("LoadDivGraph failed : %s\n", filename.c_str());
			return error;
		}

		//マップにキーとデータを登録
		map_texture_.emplace(_filename, tmp_handle);

		return 0;
	}

	sound::WAVEReader ResourceServer::LoadSound(std::string _filename)
	{
		//キーの検索
		auto iter = map_sound_.find(_filename);
		if (iter != map_sound_.end())
		{
			//キー発見
			return iter->second;
		}

		//キーがなかったら音声ファイルを読み込み
		sound::WAVEReader _wavereader;

		if (utility::GetExtension(_filename) == ".wav")
			_wavereader.LoadWAVE(_filename.c_str());

		map_sound_[_filename] = _wavereader;
		return _wavereader;
	}

	void ResourceServer::RegisterDivGraph(std::string _mode, std::string _filename, int _allnum,
		int _xnum, int _ynum, int _xsize, int _ysize, bool _async)
	{
		// キーの検索
		auto iter = map_texture_.find(_filename);
		if (iter != map_texture_.end())
		{
			//登録済みなので終了
			return;
		}

		//キーがなかったらメモリを作成
		std::vector<handle> tmp_handle(_allnum);
		std::string filename = "Resource/graphic/" + _mode + "/" + _filename;
		//非同期で画像を読み込む
		SetUseASyncLoadFlag(_async);
		int error = ::LoadDivGraph(filename.c_str(), _allnum, _xnum, _ynum, _xsize, _ysize, tmp_handle.data());
		SetUseASyncLoadFlag(FALSE);
		if (error != 0)
		{
			printfDx("LoadDivGraph failed : %s\n", filename.c_str());
			return;
		}

		if (error == 0)//成功
		{
			//マップにキーとデータを登録
			map_texture_.emplace(_filename, tmp_handle);
		}
	}

	void ResourceServer::RegisterModel(const std::string _foldername, const std::string _filename, const std::string _handlename, bool _async)
	{
		auto iter = map_model_.find(_handlename);
		if (iter != map_model_.end())
		{
			return;
		}

		std::string filename = "Resource/model/" + _foldername + "/" + _filename + ".mv1";
		SetUseASyncLoadFlag(_async);
		handle tmp = ::MV1LoadModel(filename.c_str());
		SetUseASyncLoadFlag(FALSE);
		if (tmp == -1)
		{
			printfDx("LoadMV1Model failed : %s\n", filename.c_str());
			return;
		}

		//マップにキーとデータを登録
		map_model_.emplace(_handlename, tmp);
	}

	int ResourceServer::GetModelHandle(const std::string _handlename)
	{
		auto iter = map_model_.find(_handlename.c_str());
		if (iter == map_model_.end())
		{
			printfDx("can not find model handle : %s\n", _handlename.c_str());
			return -1;
		}
		else
			return iter->second;
	}

	int ResourceServer::GetTexture(const std::string _filename, int tex_num)
	{
		//マップから画像ハンドルを探す
		auto iter = map_texture_.find(_filename);
		if (iter == map_texture_.end())
		{
			printfDx("can not find graph handle : %s\n", _filename.c_str());
			return -1;
		}
		auto handle = iter->second.at(tex_num);
		return handle;
	}
}