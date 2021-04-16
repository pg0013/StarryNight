#include <cstddef>
#include "DxLib.h"
#include "ModeServer.h"

namespace mode
{
	std::shared_ptr<ModeServer> ModeServer::modeserver_instance_ = nullptr;

	ModeServer::ModeServer()
	{
		uid_count_ = 1;
		now_mode_ = nullptr;
		skip_processmode_ = nullptr;
		skip_rendermode_ = nullptr;
		pause_processmode_ = nullptr;
	}

	ModeServer::~ModeServer()
	{
		Clear();
		modeserver_instance_ = nullptr;
	}

	int ModeServer::Add(std::shared_ptr<ModeBase> _mode, const int& _layer, const char* _name)
	{
		addmode_list_.push_back(_mode);
		_mode->uid_ = uid_count_;
		uid_count_++;
		_mode->layer_ = _layer;
		_mode->modename_ = _name;

		return _mode->uid_;
	}

	int ModeServer::Del(std::shared_ptr<ModeBase> _mode)
	{
		delmode_list_.push_back(_mode);
		return 0;
	}

	int ModeServer::Release(std::shared_ptr<ModeBase> _mode)
	{
		for (auto iter = mode_list_.begin(); iter != mode_list_.end();)
		{
			//指定モードがリストにあったら削除
			if ((*iter) == _mode)
			{
				(*iter)->Terminate();
				iter->reset();
				iter = mode_list_.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		return 1;
	}

	void ModeServer::Clear()
	{
		//モードリストの要素をすべて削除
		for (auto iter = mode_list_.rbegin(); iter != mode_list_.rend(); ++iter)
		{
			(*iter)->Terminate();
			iter->reset();
		}
		//追加リストのモードをすべて削除
		for (auto&& iteradd : addmode_list_)
		{
			iteradd->Terminate();
			iteradd.reset();
		}

		mode_list_.clear();
		addmode_list_.clear();
		delmode_list_.clear();
	}

	bool ModeServer::IsDelRegist(std::shared_ptr<ModeBase> _mode)
	{
		if (delmode_list_.size() > 0)
		{
			for (auto&& iterdel : delmode_list_)
			{
				if (iterdel == _mode) { return true; }
			}
		}
		return false;
	}

	bool ModeServer::IsAdd(std::shared_ptr<ModeBase> _mode)
	{
		//登録中のもの、登録予約中のものから検索する
		for (auto&& iter : mode_list_)
		{
			//削除予約されていないモードか？
			if (!IsDelRegist(iter) && (iter) == _mode) { return true; }
		}
		for (auto&& iteradd : addmode_list_)
		{
			//削除予約されていないモードか？
			if (!IsDelRegist(iteradd) && (iteradd) == _mode) { return true; }
		}
		return false;
	}

	std::shared_ptr<ModeBase> ModeServer::Get(const int& _uid)
	{
		//登録中のもの、登録予約中のものから検索する
		for (auto&& iter : mode_list_)
		{
			//削除予約されていないモードか？
			if (!IsDelRegist(iter) && (iter)->uid_ == _uid) { return (iter); }
		}
		for (auto&& iteradd : addmode_list_)
		{
			//削除予約されていないモードか？
			if (!IsDelRegist(iteradd) && (iteradd)->uid_ == _uid) { return (iteradd); }
		}
		return nullptr;
	}

	std::shared_ptr<ModeBase> ModeServer::Get(const char* _name)
	{
		//登録中のもの、登録予約中のものから検索する
		for (auto&& iter : mode_list_)
		{
			//削除予約されていないモードか？
			if (!IsDelRegist(iter) && (iter)->modename_ == _name) { return (iter); }
		}
		for (auto&& iteradd : addmode_list_)
		{
			//削除予約されていないモードか？
			if (!IsDelRegist(iteradd) && (iteradd)->modename_ == _name) { return (iteradd); }
		}
		return nullptr;
	}

	int ModeServer::GetId(std::shared_ptr<ModeBase> _mode)
	{
		if (IsAdd(_mode)) { return _mode->uid_; }
		return 0;
	}

	int ModeServer::GetId(const char* _name)
	{
		return GetId(Get(_name));
	}

	const char* ModeServer::GetName(std::shared_ptr<ModeBase> _mode)
	{
		if (IsAdd(_mode)) { return _mode->modename_.c_str(); }
		return nullptr;
	}

	const char* ModeServer::GetName(const int& _uid)
	{
		return GetName(Get(_uid));
	}

	int ModeServer::ProcessInit()
	{
		//削除予約されていたものを削除
		if (delmode_list_.size() > 0)
		{
			for (auto&& iterdel : delmode_list_)
			{
				Release(iterdel);
			}
			delmode_list_.clear();
		}

		//Add()されたものを有効にし、必要であればソートを行う
		if (addmode_list_.size() > 0)
		{
			for (auto&& iteradd : addmode_list_)
			{
				iteradd->Initialize();
				mode_list_.push_back(iteradd);
			}
			addmode_list_.clear();

			//レイヤーによるソートを行う
			mode_list_.sort(ModeSort);
		}

		//skip,pauseの解除
		skip_processmode_ = nullptr;
		skip_rendermode_ = nullptr;
		pause_processmode_ = nullptr;
		return 0;
	}

	int ModeServer::Process()
	{
		//現在の時間を取得
		unsigned int time = GetNowCount();
		bool pause = false;

		//レイヤーの上の方から処理
		for (auto iter = mode_list_.rbegin(); iter != mode_list_.rend(); ++iter)
		{
			if (IsDelRegist(*iter) == false)
			{
				//呼び出し中のモードを設定
				now_mode_ = (*iter);

				if (pause == false)
				{
					//時間を更新
					(*iter)->StepTime(time);
				}

				(*iter)->Process();

				//Modeが無い場合(Reboot時)
				if (mode_list_.empty()) { break; }

				if (pause == false)
				{
					//カウンタを更新
					(*iter)->StepCount();
				}
			}

			if (skip_processmode_ == (*iter)) { break; }
			if (pause_processmode_ == (*iter)) { pause = true; }
		}
		now_mode_ = nullptr;

		return 0;
	}

	int ModeServer::ProcessFinish()
	{
		return 0;
	}

	int ModeServer::RenderInit()
	{
		//画面の描画を消去し、描画先を裏画面に設定
		ClearDrawScreen();
		SetDrawScreen(DX_SCREEN_BACK);
		return 0;
	}

	int ModeServer::Render()
	{
		//レイヤーの下から処理
		for (auto&& iter : mode_list_)
		{
			if (skip_rendermode_ && skip_rendermode_ != iter) { continue; }
			skip_rendermode_ = nullptr;

			if (IsDelRegist(iter) == false)
			{
				now_mode_ = (iter);
				(iter)->Render();
			}
		}

		now_mode_ = nullptr;
		return 0;
	}

	int ModeServer::RenderFinish()
	{
		//裏画面を反転
		ScreenFlip();

		return 0;
	}

	int ModeServer::SkipProcessUnderLayer()
	{
		skip_processmode_ = now_mode_;
		return 0;
	}

	int ModeServer::SkipRenderUnderLayer()
	{
		skip_rendermode_ = now_mode_;
		return 0;
	}

	int ModeServer::PauseProcessUnderLayer()
	{
		pause_processmode_ = now_mode_;
		return 0;
	}
}